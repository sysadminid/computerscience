from cs50 import get_int


def main():
    # Asking for height of pyramid that must be height < 1 or height > 8
    h = get_height()
    pyramid(h)


def get_height():
    n = 0
    while n < 1 or n > 8:
        n = get_int("Height: ")
    return n


def pyramid(height):
    for i in range(height):

        # Print for whitespaces on the left side
        for j in range(i, height - 1):
            print(" ", end="")

        # Print for left side pyramid
        for k in range(i + 1):
            print("#", end="")

        # Print for gap between two pyramids
        print("  ", end="")

        # Print for right side pyramid
        for l in range(i + 1):
            print("#", end="")
        print()


if __name__ == "__main__":
    main()
