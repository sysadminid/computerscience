from cs50 import get_string


def main():
    text = get_string("Text: ")

    letters = count_letter(text)
    words = count_word(text)
    sentences = count_sentence(text)

    grade = coleman_liau_index(letters, words, sentences)
    print(grade)


def count_letter(text):
    return len([letter for letter in text if letter.isalpha()])


def count_word(text):
    return len(text.split())


def count_sentence(text):
    return text.count('.') + text.count('!') + text.count('?')


def coleman_liau_index(letters, words, sentences):
    index = 0.0588 * (letters / words * 100 * 1.0) - 0.296 * (sentences / words * 100) - 15.8

    if index < 1:
        return "Before Grade 1"
    elif index >= 1 and index <= 15:
        return f"Grade {round(index)}"
    else:
        return "Grade 16+"


if __name__ == "__main__":
    main()
