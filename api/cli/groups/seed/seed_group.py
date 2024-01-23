from datetime import datetime

from cli.groups import BaseGroup
from lib_objects import bcrypt
from database.schemas import User


class SeedGroup(BaseGroup):
    """ Implements methods responsible for seeding database. """
    bcrypt = bcrypt

    def init(self):
        admin = User(username='admin', pw_hash=self.bcrypt.generate_password_hash('admin'))

        self.db.session.add(admin)
        self.db.session.commit()
        print('Database seeded!')

    def users(self):
        user1 = User(username='test1', pw_hash=bcrypt.generate_password_hash('test'))
        user2 = User(username='test2', pw_hash=bcrypt.generate_password_hash('test'))
        user3 = User(username='test3', pw_hash=bcrypt.generate_password_hash('test'))

        self.db.session.add(user1)
        self.db.session.add(user2)
        self.db.session.add(user3)
        self.db.session.commit()
        print('Database seeded!')
