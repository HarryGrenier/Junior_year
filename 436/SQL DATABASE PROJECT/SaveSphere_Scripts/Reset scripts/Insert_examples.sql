-- Adding a user to the Users table
INSERT INTO Users (Name, Email, Password, DateJoined)
VALUES ('John Doe', 'john.doe@email.com', 'hashed_password123', '2023-10-31');

-- Adding a group to the Groups table
INSERT INTO Groups (GroupName, Description)
VALUES ('Savings Group', 'A group dedicated to saving money together.');

-- Adding an expense for the user
INSERT INTO Expenses (UserID, Amount, Category, Date)
VALUES (1, 100.50, 'Groceries', '2023-10-31');

-- Adding a saving entry for the user
INSERT INTO Savings (UserID, Amount, Purpose, Date)
VALUES (1, 150.75, 'Emergency Fund', '2023-10-31');

-- Adding a challenge to the Challenges table
INSERT INTO Challenges (Name, Description, StartDate, EndDate, TargetAmount)
VALUES ('30 Day Savings Challenge', 'Save as much as you can in 30 days', '2023-11-01', '2023-11-30', 500);

-- Associating the user with a group
INSERT INTO UserGroups (UserID, GroupID)
VALUES (1, 1);

-- Associating the user with a challenge
INSERT INTO UserChallenges (UserID, ChallengeID)
VALUES (1, 1);
