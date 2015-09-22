# !/usr/bin/python
# -*- coding: utf-8 -*-

import MySQLdb as mariadb
import tml  # TML low level API
import tml.constants  # constant values
import tml.common  # helper functions
import tml.core  # TMLCore()
import tml.profile  # TMLProfile()
import tml.stream  # stream classes
import tml.command  # TMLCmd()


class Server:
    profileid = "urn:your-domain:com:Android01"


    # load all the items from database - handler
    def cmd_01_load(self, aprofile, cmd):
        print("cmd01 received")

        cursor = self.__db.cursor()
        cursor.execute("SELECT * FROM items;")
        cmd.data['db']['size'] = str(cursor.rowcount)
        i = 0  # important, for the app expects it like this
        for dbid, title, body in cursor:
            cmd.data[str(i)]['id'] = str(dbid)
            cmd.data[str(i)]['title'] = title
            cmd.data[str(i)]['body'] = body
            i += 1

    # insert item into database - handler
    def cmd_02_insert(self, aprofile, cmd):
        print("cmd02 received")
        cursor = self.__db.cursor()
        title = (cmd.data['input']['title'])
        body = (cmd.data['input']['body'])
        cursor.execute("INSERT INTO items VALUES (null,%s,%s);", (title, body))
        self.__db.commit()

    # update item from database - handler
    def cmd_03_update(self, aprofile, cmd):
        print("cmd03 received")
        cursor = self.__db.cursor()
        cursor.execute("UPDATE items SET title=%s, body=%s WHERE id = %s;", (cmd.data['input']['title'],
                                                                               cmd.data['input']['body'],
                                                                               cmd.data['input']['id']))
        self.__db.commit()

    # delete item from database - handler
    def cmd_04_delete(self, aprofile, cmd):
        print("cmd04 received")
        cursor = self.__db.cursor()
        cursor.execute("DELETE FROM items WHERE id = %s;" % str(cmd.data['input']['id']))
        self.__db.commit()


    # init tml core, profile and connection to database
    def __init__(self, host="127.0.0.1", user='root', passwd="bumblebee", db='tmlexample'):
        self.__db = mariadb.connect(host=host,user=user, passwd=passwd, db= db, use_unicode=True, charset='utf8')
        self.__tmlcore = tml.core.TMLCore()
        self.__tmlcore.listener_binding = "0.0.0.0:4711"

        # register profile
        self.__tmlcore.profiles.add_profile(self.profileid)

        # register commands
        self.__tmlcore.profiles[self.profileid].register_cmd(1, self.cmd_01_load)
        self.__tmlcore.profiles[self.profileid].register_cmd(2, self.cmd_02_insert)
        self.__tmlcore.profiles[self.profileid].register_cmd(3, self.cmd_03_update)
        self.__tmlcore.profiles[self.profileid].register_cmd(4, self.cmd_04_delete)
        self.__tmlcore.listener_enabled = True
        print("listener enabled")

    #closes opened connections
    def deinit(self):
        self.__tmlcore.close()
        self.__db.close()


def run():
    while True:
        c = tml.common.getch()
        if isinstance(c, str):
            c = c.encode()
        # terminate on CTRL+C
        if c in [b'\x03', b'\x18']:
            break


def main():
    server = Server()
    run()
    server.deinit()

if __name__ == '__main__':
    main()
