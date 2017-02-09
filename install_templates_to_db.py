import mysql.connector
import argparse


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Merge templates from hdd to database")
    parser.add_argument('-h', '--host', type=str, default="localhost", help="database host" )
    parser.add_argument('-d', '--database', type=str, required=True, help="database name" )
    parser.add_argument('-u', '--user', type=str, required=True, help="database user" )
    parser.add_argument('-p', '--password', type=str, required=True, help="database user password" )

    args = parser.parse_args()

    try:
        cnx = mysql.connector.connect(user=args.user, password=args.password, host=args.host, database=args.database)
        cursor = cnx.cursor()

    except mysql.connector.Error as err:
        print(err)

    


    add_employee = ("INSERT INTO employees (first_name, last_name, hire_date, gender, birth_date) VALUES (%s, %s, %s, %s, %s)")
    cursor.execute(add_employee, data_employee)
    emp_no = cursor.lastrowid


    cnx.close()
