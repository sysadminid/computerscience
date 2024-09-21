from cs50 import get_int


def main():
    card_num = get_int("Number: ")

    if valid_card_number(card_num):
        print_card_name(card_num)
    else:
        print("INVALID")


def valid_card_number(card_num):
    card_len = len(str(card_num))

    if card_len < 13 or card_len > 16:
        return False

    checksum = 0
    for counter, num in enumerate(reversed(str(card_num))):
        if counter % 2 == 0:
            checksum += int(num)
        else:
            for i in str(int(num) * 2):
                checksum += int(i)

    if checksum % 10 == 0:
        return True
    else:
        return False


def print_card_name(card_num):
    card_len = len(str(card_num))
    start_digit = str(card_num)[:2]

    if (start_digit == "34" or start_digit == "37") and card_len == 15:
        print("AMEX")
    elif start_digit >= "51" and start_digit <= "55" and card_len == 16:
        print("MASTERCARD")
    elif start_digit[:1] == "4" and (card_len == 13 or card_len == 16):
        print("VISA")
    else:
        print("INVALID")


if __name__ == "__main__":
    main()
