import random
from faker import Faker
fake = Faker()

def generate_insert_statement(table, values):
    columns = ', '.join(values.keys())
    values_str = ', '.join("'" + str(value).replace("'", "''") + "'" for value in values.values())
    return f"INSERT INTO {table} ({columns}) VALUES ({values_str});\n"

def generate_users(num_users):
    return [
        {
            'Name': fake.name(),
            'Email': fake.email(),
            'Password': fake.password(),
            'DateJoined': fake.date_this_decade(),
            'TotalSavings': round(random.uniform(0, 10000), 2),
            'TotalExpenses': round(random.uniform(0, 10000), 2)
        }
        for _ in range(num_users)
    ]

def generate_groups(num_groups):
    return [
        {
            'GroupName': fake.word(),
            'Description': fake.text(),
            'GroupGoal': round(random.uniform(0, 10000), 2),
            'CurrentGroupSavings': round(random.uniform(0, 10000), 2)
        }
        for _ in range(num_groups)
    ]

def generate_expenses(num_expenses, num_users):
    return [
        {
            'UserID': random.randint(1, num_users),
            'Amount': round(random.uniform(0, 1000), 2),
            'Category': fake.word(),
            'Date': fake.date_this_year()
        }
        for _ in range(num_expenses)
    ]

def generate_savings(num_savings, num_users):
    return [
        {
            'UserID': random.randint(1, num_users),
            'Amount': round(random.uniform(0, 1000), 2),
            'Purpose': fake.word(),
            'Date': fake.date_this_year()
        }
        for _ in range(num_savings)
    ]

def generate_challenges(num_challenges):
    return [
        {
            'Name': fake.word(),
            'Description': fake.text(),
            'StartDate': fake.date_this_year(),
            'EndDate': fake.date_this_year(),
            'TargetAmount': round(random.uniform(0, 10000), 2)
        }
        for _ in range(num_challenges)
    ]

def generate_user_groups(num_user_groups, num_users, num_groups):
    return [
        {
            'UserID': random.randint(1, num_users),
            'GroupID': random.randint(1, num_groups)
        }
        for _ in range(num_user_groups)
    ]

def generate_user_challenges(num_user_challenges, num_users, num_challenges):
    return [
        {
            'UserID': random.randint(1, num_users),
            'ChallengeID': random.randint(1, num_challenges)
        }
        for _ in range(num_user_challenges)
    ]

if __name__ == '__main__':
    num_users = int(input('Enter the number of Users: '))
    num_groups = int(input('Enter the number of Groups: '))
    num_expenses = int(input('Enter the number of Expenses: '))
    num_savings = int(input('Enter the number of Savings: '))
    num_challenges = int(input('Enter the number of Challenges: '))
    num_user_groups = int(input('Enter the number of UserGroups: '))
    num_user_challenges = int(input('Enter the number of UserChallenges: '))

    users_data = generate_users(num_users)
    groups_data = generate_groups(num_groups)
    expenses_data = generate_expenses(num_expenses, num_users)
    savings_data = generate_savings(num_savings, num_users)
    challenges_data = generate_challenges(num_challenges)
    user_groups_data = generate_user_groups(num_user_groups, num_users, num_groups)
    user_challenges_data = generate_user_challenges(num_user_challenges, num_users, num_challenges)

    with open('mock_data.sql', 'w') as f:
        for user in users_data:
            f.write(generate_insert_statement('Users', user))
        for group in groups_data:
            f.write(generate_insert_statement('Groups', group))
        for expense in expenses_data:
            f.write(generate_insert_statement('Expenses', expense))
        for saving in savings_data:
            f.write(generate_insert_statement('Savings', saving))
        for challenge in challenges_data:
            f.write(generate_insert_statement('Challenges', challenge))
        for user_group in user_groups_data:
            f.write(generate_insert_statement('UserGroups', user_group))
        for user_challenge in user_challenges_data:
            f.write(generate_insert_statement('UserChallenges', user_challenge))

    print('Mock data generated and stored in mock_data.sql')
