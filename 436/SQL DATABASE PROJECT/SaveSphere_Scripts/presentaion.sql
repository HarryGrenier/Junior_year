-- Presentation SQL Script

-- ############################################################################################################################
-- # DDL Commands
-- ############################################################################################################################

-- ## ALTER TABLE
-- # Adding a new column to the Users table
ALTER TABLE Users 
ADD COLUMN IsActive BOOLEAN DEFAULT TRUE;

-- # Showing the changes
SELECT 
  * 
FROM Users;

-- # Dropping the column just added
ALTER TABLE Users 
DROP COLUMN IsActive;

-- ## DROP and CREATE TABLE
-- # Dropping the UserChallenges table
DROP TABLE UserChallenges;

-- # Creating the UserChallenges table again
CREATE TABLE UserChallenges (
  UserID INT,
  ChallengeID INT,
  PRIMARY KEY (UserID, ChallengeID),
  FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE CASCADE,
  FOREIGN KEY (ChallengeID) REFERENCES Challenges(ChallengeID) ON DELETE CASCADE
);

-- ############################################################################################################################
-- # DML Commands
-- ############################################################################################################################

-- ## INSERT
-- # Adding users to the table
INSERT INTO Users (Name, Email, Password, DateJoined, TotalSavings, TotalExpenses) 
VALUES
  ('Joe Doe', 'joe.doe@email.com', 'password123', '2023-01-01', 13000, 500),
  ('Jane Doe', 'jane.doe@email.com', 'password123', '2023-01-02', 3000, 1000);



-- ## SELECT
-- # Top ten savers
SELECT
    Name, TotalSavings, TotalExpenses, (totalsavings - totalexpenses) as Total
FROM
    Users
ORDER BY
    Total DESC
LIMIT 10;

-- # Find users in the negitve
SELECT Name, TotalSavings, TotalExpenses, (totalsavings - totalexpenses) as Total
FROM Users
WHERE TotalSavings - TotalExpenses < 0;

-- # Counting the number of users whose names start with H
SELECT 
  COUNT(*) AS UsersStartingWithH 
FROM Users 
WHERE Name LIKE 'H%';

SELECT *
FROM Users 
WHERE Name LIKE 'H%';

-- ## DELETE
-- # Deleting all users whose names start with H (note: CASCADE will handle related data)
DELETE FROM Users 
WHERE Name LIKE 'H%';


-- # Remove a challange
DELETE FROM Challenges 
WHERE ChallengeID = 1;

select * from challenges;


-- ## UPDATE
-- # Updating TotalSavings for certain users

-- # Counting users with TotalSavings between 8000 and 10000
SELECT 
  COUNT(*) AS UsersWithSavingsBetween8000And10000 
FROM Users 
WHERE TotalSavings > 8000 
AND TotalSavings < 10000;

-- # Update all users between 8000 and 10000 to be == 10000
UPDATE Users 
SET TotalSavings = 10000 
WHERE TotalSavings > 8000 
AND TotalSavings < 10000;

-- # Checking the update results
SELECT 
  COUNT(*) AS UsersWith10000Savings 
FROM Users 
WHERE TotalSavings = 10000;

-- # Counting users with TotalSavings less than 8000
SELECT 
  COUNT(*) AS UsersWithLessThan8000Savings 
FROM Users 
WHERE TotalSavings < 8000;

-- ############################################################################################################################
-- # Special Queries and Objects
-- ############################################################################################################################

-- ## Nested Sub-Queries
-- # Finding users with savings higher than the average savings
SELECT 
  * 
FROM Users 
WHERE TotalSavings > (
  SELECT AVG(TotalSavings) 
  FROM Users
);

-- # Find users who have an average amount of savings greater than the overall average savings amount across all users.
SELECT name, email
FROM Users
WHERE UserID IN (
  SELECT UserID
  FROM Savings
  GROUP BY UserID
  HAVING AVG(Amount) > (
    SELECT AVG(Amount)
    FROM Savings
  )
);

-- # Finds the average total savings across all users
SELECT
    Name,
    TotalSavings
FROM
    Users
WHERE
    TotalSavings > (
        SELECT AVG(TotalSavings)
        FROM Users
    )
 ORDER BY TotalSavings DESC;

   
-- ## Outer-Join Queries
-- # Finding all users and their group memberships, including those without a group
SELECT 
  Users.Name, 
  Groups.GroupName 
FROM Users 
LEFT JOIN UserGroups ON Users.UserID = UserGroups.UserID 
LEFT JOIN Groups ON UserGroups.GroupID = Groups.GroupID;

-- ## Inner-Join Queries
-- # Finding all users and their group memberships, excluding those without a group
SELECT 
  Users.Name, 
  Groups.GroupName 
FROM Users 
INNER JOIN UserGroups ON Users.UserID = UserGroups.UserID 
INNER JOIN Groups ON UserGroups.GroupID = Groups.GroupID
order by name;

-- # Get all users that have logged a savings or expense in the last 30 days 
SELECT DISTINCT u.Name
FROM Users u
LEFT JOIN Savings s ON u.UserID = s.UserID AND s.Date >= CURRENT_DATE - INTERVAL '30 days'
LEFT JOIN Expenses e ON u.UserID = e.UserID AND e.Date >= CURRENT_DATE - INTERVAL '30 days'
WHERE s.SavingsID IS NOT NULL OR e.ExpenseID IS NOT NULL;


-- # Total amount saved by all users in each challenge
SELECT 
  c.ChallengeID, 
  c.Name, 
  SUM(s.Amount) AS TotalSavings 
FROM Challenges c 
JOIN UserChallenges uc ON c.ChallengeID = uc.ChallengeID 
JOIN Savings s ON uc.UserID = s.UserID 
GROUP BY c.ChallengeID;

-- ############################################################################################################################
-- # Views
-- ############################################################################################################################

-- ## Creating a View
-- # Creating a view for user savings
CREATE VIEW NetSavings AS
SELECT 
    u.UserID,
    u.Name,
    (COALESCE(SUM(s.Amount), 0) - COALESCE(SUM(e.Amount), 0)) AS NetAmount
FROM Users u
LEFT JOIN Savings s ON u.UserID = s.UserID
LEFT JOIN Expenses e ON u.UserID = e.UserID
GROUP BY u.UserID, u.Name;

-- # Showing data from the view
SELECT 
  * 
FROM NetSavings;

SELECT *
FROM NetSavings
WHERE NetAmount > 0;

-- # Dropping the view
DROP VIEW IF EXISTS NetSavings;

-- ## Check Constraints
-- # Adding a CHECK constraint to ensure TotalSavings is always greater than or equal to 0
ALTER TABLE Users 
ADD CONSTRAINT chk_totalsavings CHECK (TotalSavings >= 0);

-- # Showing the CHECK constraint
SELECT 
  * 
FROM information_schema.check_constraints 
WHERE constraint_name = 'chk_totalsavings';

-- # Dropping the CHECK constraint
ALTER TABLE Users 
DROP CONSTRAINT chk_totalsavings;




-- ############################################################################################################################
-- # Clean Up
-- ############################################################################################################################

