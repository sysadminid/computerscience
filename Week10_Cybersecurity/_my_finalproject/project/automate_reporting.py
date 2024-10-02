import datetime
import json
import os
import requests
import time
from google.oauth2.service_account import Credentials
from googleapiclient.discovery import build
from googleapiclient.http import MediaFileUpload

# Grafana API settings
GRAFANA_URL = "<GRAFANA_URL"
GRAFANA_API_KEY = "<GRAFANA API KEY>"
DATASOURCE_ID = <DATASOURCE_ID>
DASHBOARD_UID = "<DASHBOARD_UID>"

# Google Sheets API settings
SCOPES = ['https://www.googleapis.com/auth/spreadsheets', 'https://www.googleapis.com/auth/drive']
SAC_CREDENTIAL_FILE = '<PATH_TO_CREDENTIALS_JSON_FILE>'
SPREADSHEET_ID = '<SPREADSHEET_ID>'
SHEET_GID = '<SHEET_GID>'
SHEET_NAME = '<SHEET_NAME>'

# Google Drive settings
FOLDER_ID = '<GDRIVE_FOLDER_ID>'

# Checkpoint file
CHECKPOINT_FILE = 'checkpoint.json'

# Adjust range time scraping on `generate_image_url` function.

def get_projects_and_vms(GRAFANA_URL, GRAFANA_API_KEY, DATASOURCE_ID):
    headers = {'Authorization': f'Bearer {GRAFANA_API_KEY}'}

    def get_projects():
        endpoint = f"{GRAFANA_URL}/api/datasources/{DATASOURCE_ID}/resources/zabbix-api"
        data = {
            "datasourceId": DATASOURCE_ID,
            "method": "hostgroup.get",
            "params": {
                "output": ["name", "groupid"],
                "sortfield": "name",
                "real_hosts": True
            }
        }

        response = requests.post(endpoint, headers=headers, data=json.dumps(data))
        response.raise_for_status()
        return response.json()["result"]

    def get_vms(group_ids):
        endpoint = f"{GRAFANA_URL}/api/datasources/{DATASOURCE_ID}/resources/zabbix-api"
        data = {
            "datasourceId": DATASOURCE_ID,
            "method": "host.get",
            "params": {
                "output": ["hostid", "name", "host"],
                "sortfield": "name",
                "groupids": group_ids
            }
        }

        response = requests.post(endpoint, headers=headers, data=json.dumps(data))
        response.raise_for_status()
        return response.json()["result"]

    # Main logic
    projects = get_projects()
    output = []

    for project in projects:
        group_id = project["groupid"]
        group_name = project["name"]
        vms = get_vms([group_id])
        for vm in vms:
            output.append({
                "project": group_name,
                "vms": [vm["name"]]
            })

    return output


def generate_image_url(project, vm):
    from_time = int(time.time() * 1000) - 30 * 24 * 60 * 60 * 1000  # 30 days ago until now
    to_time = int(time.time() * 1000)

    url = f"{GRAFANA_URL}/render/d-solo/{DASHBOARD_UID}/bsd-disk-usage"
    params = {
        "orgId": 1,
        "var-project": project,
        "var-vm": vm,
        "from": from_time,
        "to": to_time,
        "panelId": 1,
        "width": 1000,
        "height": 500,
        "tz": "Asia/Jakarta"
    }
    return url + "?" + "&".join([f"{k}={v}" for k, v in params.items()])


def resize_row(sheet_service, spreadsheet_id, row, height):
    """Resize the row height in Google Sheets."""
    requests = [
        {
            "updateDimensionProperties": {
                "range": {
                    "sheetId": SHEET_GID,
                    "dimension": "ROWS",
                    "startIndex": row - 1,
                    "endIndex": row
                },
                "properties": {
                    "pixelSize": height
                },
                "fields": "pixelSize"
            }
        }
    ]
    body = {
        'requests': requests
    }
    sheet_service.spreadsheets().batchUpdate(
        spreadsheetId=spreadsheet_id,
        body=body
    ).execute()

def sanitize_filename(filename):
    """Replace any special characters in the filename."""
    return filename.replace('/', '_').replace('\\', '_')

def download_image(url, filename):
    response = requests.get(url, headers={"Authorization": f"Bearer {GRAFANA_API_KEY}"})
    if response.status_code == 200:
        with open(filename, 'wb') as f:
            f.write(response.content)
    else:
        print(f"Failed to download image: {response.status_code}")


def get_existing_file_id(drive_service, FOLDER_ID, file_name):
    """Check if the file already exists in the specified folder and return its ID."""
    query = f"'{FOLDER_ID}' in parents and name='{file_name}'"
    results = drive_service.files().list(q=query, fields="files(id)").execute()
    files = results.get('files', [])
    if files:
        return files[0].get('id')
    return None


def replace_image(drive_service, file_id, filename):
    """Replace an existing file on Google Drive with a new one."""
    media = MediaFileUpload(filename, resumable=True)
    drive_service.files().update(
        fileId=file_id,
        media_body=media
    ).execute()


def upload_to_google_sheets(filename, project, vm, row, number):
    # Load service account credentials
    creds = Credentials.from_service_account_file(SAC_CREDENTIAL_FILE, scopes=SCOPES)

    # Initialize the Google Sheets and Drive API clients
    service = build('sheets', 'v4', credentials=creds)
    drive_service = build('drive', 'v3', credentials=creds)

    # Upload image to Google Drive in the specific folder
    file_name = f'{project}_{vm}_disk_usage.png'
    existing_file_id = get_existing_file_id(drive_service, FOLDER_ID, file_name)

    if existing_file_id:
        # Replace the existing image
        replace_image(drive_service, existing_file_id, filename)
        file_id = existing_file_id
    else:
        # Upload a new image
        file_metadata = {
            'name': file_name,
            'parents': [FOLDER_ID]
        }
        media = MediaFileUpload(filename, resumable=True)
        file = drive_service.files().create(body=file_metadata, media_body=media, fields='id').execute()
        file_id = file.get('id')

    # Make the file publicly accessible
    drive_service.permissions().create(
        fileId=file_id,
        body={
            'role': 'reader',
            'type': 'anyone',
        }
    ).execute()

    # Generate the link to the image
    image_url = f"https://drive.google.com/uc?export=view&id={file_id}"

    # Update Google Sheets with project and VM name
    sheet = service.spreadsheets()
    sheet.values().update(
        spreadsheetId=SPREADSHEET_ID,
        range=f'{SHEET_NAME}!A{row}:D{row}',
        valueInputOption='USER_ENTERED',
        body={'values': [[number, project, vm, f'=IMAGE("{image_url}"; 1)']]}
    ).execute()

    # Resize row to fit the image
    resize_row(service, SPREADSHEET_ID, row, 230)


def save_checkpoint(row, project, vm):
    with open(CHECKPOINT_FILE, "w") as f:
        json.dump({"row": row, "project": project, "vm": vm, "date": str(datetime.date.today())}, f)


def load_checkpoint():
    if os.path.exists(CHECKPOINT_FILE):
        with open(CHECKPOINT_FILE, "r") as f:
            return json.load(f)
    return {"row": 2, "project": None, "vm": None, "date": str(datetime.date.today())}


def reset_checkpoint():
    if os.path.exists(CHECKPOINT_FILE):
        os.remove(CHECKPOINT_FILE)


def main():
    checkpoint = load_checkpoint()

    if checkpoint["date"] != str(datetime.date.today()):
        reset_checkpoint()
        checkpoint = {"row": 2, "project": None, "vm": None, "date": str(datetime.date.today())}

    # Initialize Google Sheets
    creds = Credentials.from_service_account_file(SAC_CREDENTIAL_FILE,
                                                  scopes=['https://www.googleapis.com/auth/spreadsheets'])
    service = build('sheets', 'v4', credentials=creds)
    sheet = service.spreadsheets()

    # Set up headers
    sheet.values().update(
        spreadsheetId=SPREADSHEET_ID,
        range=f'{SHEET_NAME}!A1:D1',
        valueInputOption='USER_ENTERED',
        body={'values': [['No', 'Project Name', 'VM Name', 'Root Disk Usage']]}
    ).execute()

    if checkpoint["row"] == 2:
        sheet.values().update(
            spreadsheetId=SPREADSHEET_ID,
            range=f'{SHEET_NAME}!A1:D1',
            valueInputOption='USER_ENTERED',
            body={'values': [['No', 'Project Name', 'VM Name', 'Root Disk Usage']]}
        ).execute()

    projects_and_vms = get_projects_and_vms(GRAFANA_URL, GRAFANA_API_KEY, DATASOURCE_ID)

    row = checkpoint["row"]
    start_project = checkpoint["project"]
    start_vm = checkpoint["vm"]
    started = False

    for project_data in projects_and_vms:
        project = project_data['project']
        if start_project and project != start_project:
            continue
        for vm in project_data['vms']:
            if start_vm and vm != start_vm:
                continue
            started = True

            # Generate the image URL and download the image
            url = generate_image_url(project, vm)
            filename = f"{project}_{vm}_disk_usage.png"
            filename = sanitize_filename(filename)
            download_image(url, filename)

            # Upload to Google Sheets and Google Drive
            upload_to_google_sheets(filename, project, vm, row, row - 1)

            # Clean up the local file
            os.remove(filename)

            # Save the checkpoint
            save_checkpoint(row, project, vm)

            # Move to the next row
            row += 1

            # Clear start_vm after processing the first VM
            start_vm = None

        # Clear start_project after processing the first project
        start_project = None

    # Once completed, remove the checkpoint file
    reset_checkpoint()


if __name__ == "__main__":
    main()
