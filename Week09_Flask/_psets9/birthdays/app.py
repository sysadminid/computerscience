import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    fetchBirthdays = db.execute("SELECT * FROM birthdays")

    # Track which birthday is being edited (if any)
    edit_id = None

    if request.method == "POST":
        action = request.form.get("action")
        id = request.form.get("id")

        if action == "add":
            # Handle adding a new birthday
            name = request.form.get("name")
            month = int(request.form.get("month"))
            day = int(request.form.get("day"))

            # Validate input
            if not name or not month or not day or month < 1 or month > 12 or day < 1 or day > 31:
                message = "Invalid input. Please check the month and day."
                return render_template("index.html", birthdays=fetchBirthdays, message=message)

            # Insert new birthday into the database
            db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, month, day)
            return redirect("/")

        elif action == "delete":
            # Handle deleting a birthday
            db.execute("DELETE FROM birthdays WHERE id = ?", id)
            return redirect("/")

        elif action == "edit":
            # Set the birthday to edit
            edit_id = int(id)
            return render_template("index.html", birthdays=fetchBirthdays, edit_id=edit_id)

        elif action == "save":
            # Save the edited birthday
            name = request.form.get("name")
            month = request.form.get("month")
            day = request.form.get("day")

            # Validate input
            if not name or not month or not day or int(month) < 1 or int(month) > 12 or int(day) < 1 or int(day) > 31:
                message = "Invalid input. Please check the month and day."
                return render_template("index.html", birthdays=fetchBirthdays, message=message, edit_id=int(id))

            # Update the birthday in the database
            db.execute("UPDATE birthdays SET name = ?, month = ?, day = ? WHERE id = ?",
                       name, month, day, id)
            return redirect("/")

        elif action == "cancel":
            # Cancel editing (just redirect without saving)
            return redirect("/")

    else:
        # Render the main page with all birthdays
        return render_template("index.html", birthdays=fetchBirthdays, edit_id=edit_id)
