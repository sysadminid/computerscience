import os
import uuid

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query for balance the user
    balance = db.execute(
        """
        SELECT cash
        FROM users
        WHERE id = ?
        """,
        session.get('user_id')
    )

    # Lookup to user's assets
    assets = db.execute(
        """
        SELECT * FROM (
            SELECT symbol, SUM(shares) AS shares
            FROM transactions
            WHERE user_id = ?
            GROUP BY symbol
            )
        WHERE shares > 0
        """,
        session.get('user_id')
    )

    # Store balance and total portofolio on variables
    balance = balance[0]['cash']
    total = 0 + balance

    # Iterate over user's assets to count price stock, total price of stocks, & total portofolio
    for asset in assets:
        asset['price'] = "{:.2f}".format(lookup(asset['symbol'])['price'])
        asset['total'] = "{:.2f}".format(float(asset['price']) * asset['shares'])

        total += float(asset['total'])

        asset['price'] = usd(float(asset['price']))
        asset['total'] = usd(float(asset['total']))

    return render_template("index.html", assets=assets, cash=usd(balance), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("invalid shares", 400)

        symbol_form = request.form.get("symbol")
        user_id = session.get('user_id')

        # Query for user balance
        balance = db.execute(
            """
            SELECT cash
            FROM users
            WHERE id = ?
            """,
            user_id
        )

        # Lookup to stock that user want to buy
        quote = lookup(symbol_form)

        # If lookup to stock found None returned
        if quote == None:
            return apology("invalid symbol", 400)

        # Capture stock price and total
        price = quote['price']
        total = price * shares

        # Validation buy form
        if not symbol_form:
            return apology("missing symbol", 400)
        elif shares < 0:
            return apology("invalid shares", 400)
        elif shares == 0:
            return apology("too few shares", 400)
        elif balance[0]['cash'] < (price * shares):
            return apology("can't afford", 400)
        elif quote != None:
            # Generate uuid for transaction identity
            txn = str(uuid.uuid4())

            # Insert transaction to database
            db.execute("BEGIN TRANSACTION")
            db.execute(
                """
                INSERT INTO transactions (txn, user_id, type, symbol, shares, price)
                VALUES(?, ?, 'buy', ?, ?, ?)
                """,
                txn,
                user_id,
                quote['symbol'],
                shares,
                price
            )

            # Update balance of user
            db.execute(
                """
                UPDATE users
                SET cash = cash - ?
                WHERE id = ?
                """,
                total, user_id
            )
            db.execute("COMMIT")

            # Redirect to homepage and give flash message for stock that user bought
            flash('Bought!')
            return redirect("/")

        # If user input invalid symbol stock
        else:
            return apology("invalid symbol", 400)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query history transaction user
    assets = db.execute(
        """
        SELECT symbol, shares, price, transaction_time
        FROM transactions
        WHERE user_id = ?
        """,
        session.get('user_id')
    )

    # Iterate over user's assets to count price stock and total price of stocks
    for asset in assets:
        asset['price'] = "{:.2f}".format(lookup(asset['symbol'])['price'])
        asset['total'] = "{:.2f}".format(float(asset['price']) * asset['shares'])

        asset['price'] = usd(float(asset['price']))
        asset['total'] = usd(float(asset['total']))

    return render_template("history.html", assets=assets)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            """
            SELECT *
            FROM users
            WHERE username = ?
            """,
            request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        symbol = str(request.form.get("symbol"))

        if not symbol:
            return apology("missing symbol", 400)

        quote = lookup(symbol)

        if quote != None:
            quote['price'] = usd(quote['price'])
            return render_template("quoted.html", quote=quote)
        else:
            return apology("invalid symbol", 400)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # If the user’s input is blank
        if not username:
            return apology("must provide username", 400)
        elif not password:
            return apology("must provide password", 400)

        # If password does not match
        if password != confirmation:
            return apology("passwords doesn't match", 400)

        # Checking if username already exist
        try:
            db.execute(
                """
                INSERT INTO users (username, hash)
                VALUES(?, ?)
                """,
                username,
                generate_password_hash(request.form.get("password"))
            )
        except ValueError:
            return apology("username already taken", 400)

        # Log in directly after successfully registered
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", username)

        session["user_id"] = rows[0]["id"]
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Lookup to user's assets
    assets = db.execute(
        """
        SELECT symbol, SUM(shares) AS shares
        FROM transactions
        WHERE user_id = ?
        GROUP BY symbol
        """,
        session.get('user_id')
    )

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("invalid shares", 400)

        stock = request.form.get("symbol")

        ownShares = next(key['shares'] for key in assets if key['symbol'] == stock)
        ownStock = [key['symbol'] for key in assets]

        if not shares:
            return apology("missing shares", 400)
        elif shares > ownShares:
            return apology("too many shares", 400)
        elif not stock:
            return apology("missing symbol", 400)
        elif stock not in ownStock:
            return apology("symbol not owned", 400)

        quote = lookup(stock)

        price = quote['price']
        total = price * shares
        txn = str(uuid.uuid4())
        user_id = session.get('user_id')

        db.execute("BEGIN TRANSACTION")
        db.execute(
            """
            INSERT INTO transactions (txn, user_id, type, symbol, shares, price)
            VALUES(?, ?, 'sell', ?, -?, ?)
            """,
            txn,
            user_id,
            quote['symbol'],
            shares,
            price
        )
        db.execute(
            """
            UPDATE users
            SET cash = cash + ?
            WHERE id = ?
            """,
            total, user_id
        )
        db.execute("COMMIT")

        flash('Sold!')
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html", assets=assets)


@app.route("/changepassword", methods=["GET", "POST"])
def changepassword():
    """Change the user password"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        username = request.form.get("username"),

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 403)

        # Ensure current password was submitted
        elif not request.form.get("current"):
            return apology("must provide current password", 403)

        # Ensure new password was submitted
        elif not request.form.get("new"):
            return apology("must provide new password", 403)

        # Query database for username
        q = db.execute(
            """
            SELECT *
            FROM users
            WHERE username = ?
            """,
            username
        )

        # Ensure username exists and password is correct
        if len(q) != 1 or not check_password_hash(
            q[0]["hash"], request.form.get("current")
        ):
            return apology("invalid username and/or password", 403)

        # Change with new password
        db.execute(
            """
            UPDATE users
            SET hash = ?
            WHERE username = ?
            """,
            generate_password_hash(request.form.get("new")),
            username
        )

        return render_template("changepassword.html", message="Your password changed! Please login with your new password.")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("changepassword.html")
