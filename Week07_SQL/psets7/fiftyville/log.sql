-- Keep a log of any SQL queries you execute as you solve the mystery.


-- Find the description about crime scene and obtained some findings:
-- 1. The CS50 duck took place at 10:15am at the Humphrey Street bakery;
-- 2. In interviews with three witnesses they mentioned the bakery;
-- 3. The littering occurred at 16:36. There are no known witnesses.
SELECT description
  FROM crime_scene_reports
 WHERE day = 28
   AND month = 7
   AND year = 2023
   AND street = 'Humphrey Street';


-- Find the detail of transcripts from three witnesses that mentioned the bakery:
-- 1. Ruth: Ten minutes after the theft, Ruth saw the thief run with his car in bakery parking lot;
-- 2. Eugene: someone that Eugene's recognized & the thief withdrawing some money on Leggett Street at earlier morning;
-- 3. Raymond: the thief called someone talked about plan to take the earliest flight out of Fiftyville tomorrow and ask to the accomplice to purchase the flight ticket.
SELECT name, transcript
  FROM interviews
 WHERE day = 28
   AND month = 7
   AND year = 2023
   AND transcript LIKE '%bakery%';


-- Findings after search activity details from bakery security logs:
-- There are some activity exit between 10:15am - 10:25am
SELECT p.name, p.license_plate, p.passport_number, p.phone_number
  FROM people AS p
  JOIN bakery_security_logs AS b
    ON p.license_plate = b.license_plate
 WHERE b.day = 28
   AND b.month = 7
   AND b.year = 2023
   AND hour = 10
   AND minute >= 15
   AND minute <= 25;


-- Check into phone_calls from list suspects before
-- Get some suspects who are more narrowed down
-- |     caller     |    receiver    |
-- | (130) 555-0289 | (996) 555-8899 |
-- | (499) 555-9472 | (892) 555-8872 |
-- | (367) 555-5533 | (375) 555-8161 |
-- | (499) 555-9472 | (717) 555-1342 |
-- | (770) 555-1861 | (725) 555-3243 |
SELECT caller, receiver, duration
  FROM phone_calls
 WHERE duration < 60
   AND day = 28
   AND month = 7
   AND year = 2023
   AND (
        caller IN (
           SELECT p.phone_number
             FROM people AS p
             JOIN bakery_security_logs AS b
               ON p.license_plate = b.license_plate
            WHERE b.day = 28
              AND b.month = 7
              AND b.year = 2023
              AND b.hour = 10
              AND b.minute >= 15
              AND b.minute <= 25
        )
        OR receiver IN (
             SELECT p.phone_number
               FROM people AS p
               JOIN bakery_security_logs AS b
                 ON p.license_plate = b.license_plate
              WHERE b.day = 28
                AND b.month = 7
                AND b.year = 2023
                AND b.hour = 10
                AND b.minute >= 15
                AND b.minute <= 25
        )
   );


-- Looking for a suspected thief that has bank accounts
-- Got 2 names! One of them might be a thief.
-- | name  | passport_number |  phone_number  | account_number |
-- | Bruce | 5773159633      | (367) 555-5533 | 49610011       |
-- | Diana | 3592750733      | (770) 555-1861 | 26013199       |
SELECT people.name, people.passport_number, people.phone_number, b.account_number
  FROM people
  JOIN bank_accounts AS b
    ON people.id = b.person_id
 WHERE people.phone_number IN (
       SELECT caller
         FROM phone_calls
        WHERE duration < 60
          AND day = 28
          AND month = 7
          AND year = 2023
          AND (
               caller IN (
                  SELECT p.phone_number
                    FROM people AS p
                    JOIN bakery_security_logs AS b
                      ON p.license_plate = b.license_plate
                   WHERE b.day = 28
                     AND b.month = 7
                     AND b.year = 2023
                     AND b.hour = 10
                     AND b.minute >= 15
                     AND b.minute <= 25
               )
               OR receiver IN (
                    SELECT p.phone_number
                      FROM people AS p
                      JOIN bakery_security_logs AS b
                        ON p.license_plate = b.license_plate
                     WHERE b.day = 28
                       AND b.month = 7
                       AND b.year = 2023
                       AND b.hour = 10
                       AND b.minute >= 15
                       AND b.minute <= 25
               )
          )
      );


-- There are 2 withdraw transactions at Leggett Street's ATM on July 28th:
-- 1. Withdrew the amount of 50 by account number from Bruce
-- 2. Withdrew the amount of 35 by account number from Diana
SELECT account_number, transaction_type, amount
  FROM atm_transactions
 WHERE atm_location = 'Leggett Street'
   AND day = 28
   AND month = 7
   AND year = 2023
   AND account_number IN (
    SELECT b.account_number
      FROM people
      JOIN bank_accounts AS b
        ON people.id = b.person_id
     WHERE people.phone_number IN (
           SELECT caller
             FROM phone_calls
            WHERE duration < 60
              AND day = 28
              AND month = 7
              AND year = 2023
              AND (
                   caller IN (
                      SELECT p.phone_number
                        FROM people AS p
                        JOIN bakery_security_logs AS b
                          ON p.license_plate = b.license_plate
                       WHERE b.day = 28
                         AND b.month = 7
                         AND b.year = 2023
                         AND b.hour = 10
                         AND b.minute >= 15
                         AND b.minute <= 25
                   )
                   OR receiver IN (
                        SELECT p.phone_number
                          FROM people AS p
                          JOIN bakery_security_logs AS b
                            ON p.license_plate = b.license_plate
                         WHERE b.day = 28
                           AND b.month = 7
                           AND b.year = 2023
                           AND b.hour = 10
                           AND b.minute >= 15
                           AND b.minute <= 25
                   )
              )
          )
   );



-- Search for flights on the dates after the theft incident from
-- Fortunately, at this point we can conclude one candidate for a thief with the strongest evidence is Bruce
-- Because Bruce took the earliest flight
SELECT pep.name, p.passport_number, f.origin_airport_id, f.destination_airport_id, f.hour, f.minute
  FROM flights AS f
  JOIN passengers AS p
    ON f.id = p.flight_id
  JOIN people AS pep
    ON pep.passport_number = p.passport_number
 WHERE day = 29
   AND month = 7
   AND year = 2023
   AND p.passport_number IN (
    SELECT people.passport_number
      FROM people
      JOIN bank_accounts AS b
        ON people.id = b.person_id
     WHERE people.phone_number IN (
           SELECT caller
             FROM phone_calls
            WHERE duration < 60
              AND day = 28
              AND month = 7
              AND year = 2023
              AND (
                   caller IN (
                      SELECT p.phone_number
                        FROM people AS p
                        JOIN bakery_security_logs AS b
                          ON p.license_plate = b.license_plate
                       WHERE b.day = 28
                         AND b.month = 7
                         AND b.year = 2023
                         AND b.hour = 10
                         AND b.minute >= 15
                         AND b.minute <= 25
                   )
                   OR receiver IN (
                        SELECT p.phone_number
                          FROM people AS p
                          JOIN bakery_security_logs AS b
                            ON p.license_plate = b.license_plate
                         WHERE b.day = 28
                           AND b.month = 7
                           AND b.year = 2023
                           AND b.hour = 10
                           AND b.minute >= 15
                           AND b.minute <= 25
                   )
              )
          )
   );


-- Find for cities from airport ids 8 and 4, finally found out which city the thief escaped to!
-- Origin airport       : Fiftyville Regional Airport at Fiftyville
-- Destination airport  : LaGuardia Airport at New York City
SELECT id, full_name, city
  FROM airports
 WHERE id = 8
    OR id = 4
 ORDER BY id DESC;


-- Looking for accomplice is, based on the receiver call of Bruce's call on the day of the incident
-- FOUND! The accomplice is Robin
SELECT name
  FROM people
 WHERE phone_number = '(375) 555-8161';

