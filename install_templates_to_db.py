import mysql.connector
import argparse
import os, glob

class Template:
    def __init__(self, fileName):
        self.name = ''

        templatefile = open(fileName, 'r')
        data = templatefile.readlines()

        self.text = '\n'.join(data)

        self.name = " ".join(data[0].strip().split(' ')[1:])

    def getTemplate(self):
        return self.text

    def getName(self):
        return self.name

def listTemplates(installTemplate = False):

    baseFolder = os.getcwd()

    if installTemplate:
        baseFolder += os.path.sep + "install_templates" + os.path.sep + "*.igui"
    else:
        baseFolder += os.path.sep + "templates" + os.path.sep + "*.gui"

    print("Searching file: " + baseFolder)

    globIt = glob.iglob(baseFolder )

    vReturn = []

    for x in globIt:

        vReturn.append(x)

    return vReturn


def createUser(cnx, cursor, username, usermail):
    add_template = ("insert into users (name, email) values (%s, %s)")
    cursor.execute(add_template, (username, usermail))
    cnx.commit()

    return cursor.lastrowid


def getUserID(cnx, cursor, args, simulate):
    findUser_template = ("select id from users where name = %s and email = %s")

    setFoundUsers = set()

    if cnx != None:
        cursor.execute(findUser_template, (args.user_name, args.user_email))

        for (id, ) in cursor:
            setFoundUsers.add(id)

    if len(setFoundUsers) == 0:
        if simulate:
            return -1

        return createUser(cnx, cursor, args.user_name, args.user_email)

    return list(setFoundUsers)[0]


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Merge templates from hdd to database")
    parser.add_argument('-s', '--host', type=str, default="localhost", help="database host")
    parser.add_argument('-d', '--database', type=str, required=True, help="database name")
    parser.add_argument('-u', '--user', type=str, required=True, help="database user")
    parser.add_argument('-p', '--password', type=str, required=True, help="database user password")
    parser.add_argument('--port', type=int, default=3306, help="database port")

    parser.add_argument('-un', '--user_name', type=str, required=True, help="user name")
    parser.add_argument('-ue', '--user_email', type=str, required=True,
                        help="user email (not showed if --anonymous supplied)")
    parser.add_argument('-a', '--anonymous', action='store_true', default=False, help="submit anonymously")
    parser.add_argument('--simulate', action='store_true', default=False, help="does not create any db entries")

    args = parser.parse_args()

    cnx = None
    cursor = None
    try:
        cnx = mysql.connector.connect(user=args.user, password=args.password, host=args.host, database=args.database, port=args.port)
        cursor = cnx.cursor()

    except mysql.connector.Error as err:
        print(err)

    userid = getUserID(cnx, cursor, args, args.simulate)

    add_template = ("insert into templates (displayname, template, type, user, anonym, omictools) values (%s, %s, %s, %s, %s, '')")
    delete_existing_template = ("delete from templates where user = %s and type = %s and displayname = %s")

    install_templates = listTemplates(True)
    print("Inserting files: " + str(install_templates))

    anonym = 1 if args.anonymous == True else 0

    templateList = open("./template_list", "w")

    for templateFile in install_templates:
        template = Template(templateFile)

        insertdata = (template.getName(), template.getTemplate(), 1, userid, anonym)

        print("Inserting Install Template: " + insertdata[0])
        print("Inserting length          : " + str(len(insertdata[1])))
        print("Inserting for user        : " + str(insertdata[3]))
        print("Inserting anonymously     : " + str(insertdata[4]))

        templateList.write(template.getName() + "\n")

        if args.simulate:
             continue

        try:


            print("Delete old")
            deletedata = (insertdata[3], insertdata[2], insertdata[0])
            print(deletedata)
            cursor.execute(delete_existing_template, deletedata)

            cnx.commit()
            
            print("Insert new")
            cursor.execute(add_template, insertdata)
            cnx.commit()

        except mysql.connector.Error as err:
            print("Error:")
            print(err)
            print("in")
            print(cursor.statement)



    if cnx != None:
        cnx.close()
