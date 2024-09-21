# from werkzeug.security import check_password_hash, generate_password_hash


# password = input("Password     : ")
# confirm  = input("Confirmation : ")

# print(generate_password_hash(password))
# print(check_password_hash(confirm))
import uuid
transaction_id = str(uuid.uuid4())

print(transaction_id)
