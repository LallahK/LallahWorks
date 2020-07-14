import sqlite3
from sqlite3 import Error, Row
from sys import argv

class StudentDB(object):

    def __init__(self, db_file):
        self.conn = None
        try:
            self.conn = sqlite3.connect(db_file)
            self.conn.row_factory = Row
        except Error as e:
            print(e)

    def create_tables(self):
        try:
            sql_create_students_table = \
                """CREATE TABLE IF NOT EXISTS students (
                student_num TEXT PRIMARY KEY,
                bomb_id text NOT NULL UNIQUE,
                session_id TEXT
                );"""

            sql_create_phases_table = \
                """CREATE TABLE IF NOT EXISTS phases (
                student_num text NOT NULL,
                phase_num INTEGER NOT NULL,
                variant INTEGER NOT NULL,
                static_inputs TEXT NOT NULL,
                last_dynamic_input TEXT,
                PRIMARY KEY(student_num, phase_num)
                );"""

            c = self.conn.cursor()
            c.execute(sql_create_students_table)
            c.execute(sql_create_phases_table)

            self.conn.commit()

        except Error as e:
            print("ERROR CREATING TABLES: {}".format(e))

    def add_student(self, student_num, bomb_id, phase_info):
        """This should be called during bomb generation.

        phase_info is a dict of the form:
        { <phase_num>: NamedTuple(variant, static_inputs)}"""

        sql_add_student = \
            """INSERT INTO students(student_num, bomb_id)
            VALUES(?,?);"""

        sql_add_phase = \
            """INSERT INTO phases(student_num, phase_num, variant, static_inputs)
            VALUES(?,?,?,?)"""

        try:
            c = self.conn.cursor()

            c.execute(sql_add_student, (student_num, bomb_id))

            for phase_num in phase_info:
                variant = phase_info[phase_num].variant
                static_inputs = phase_info[phase_num].static_inputs
                c.execute(sql_add_phase, (student_num, phase_num, variant, static_inputs))

            self.conn.commit()


if __name__ == '__main__':
    if len(argv) != 2:
        print('Usage: python create_database.py <DB_FILENAME>')
    else:
        db = StudentDB(argv[1])
        db.create_tables()

