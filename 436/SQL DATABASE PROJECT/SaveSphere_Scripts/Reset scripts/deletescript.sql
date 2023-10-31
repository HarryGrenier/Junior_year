TRUNCATE TABLE UserChallenges, UserGroups, Challenges, Savings, Expenses, Groups, Users CASCADE;

ALTER SEQUENCE users_userid_seq RESTART WITH 1;
ALTER SEQUENCE groups_groupid_seq RESTART WITH 1;
ALTER SEQUENCE expenses_expenseid_seq RESTART WITH 1;
ALTER SEQUENCE savings_savingsid_seq RESTART WITH 1;
ALTER SEQUENCE challenges_challengeid_seq RESTART WITH 1;

DROP TABLE UserGroups;
DROP TABLE UserChallenges;
DROP TABLE Expenses;
DROP TABLE Savings;
DROP TABLE Users;
DROP TABLE Groups;
DROP TABLE Challenges;

