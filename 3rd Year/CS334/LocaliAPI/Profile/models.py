from django.db import models
from django.contrib.auth.models import PermissionsMixin
from django.contrib.auth.base_user import AbstractBaseUser
from django.contrib.auth.models import User
from django.utils.translation import ugettext_lazy as _
from Profile.UserManager import UserManager
import uuid ###

class User(AbstractBaseUser, PermissionsMixin):

    id = models.UUIDField(primary_key=True, default=uuid.uuid4, editable=False) ###
    username = models.CharField( unique = True, editable = False, db_index = True, max_length = 32)
    handle = models.CharField(_('handle'), max_length = 32, blank = True)
    email = models.EmailField(_('email address'))
    date_joined = models.DateTimeField(_('date joined'), auto_now_add=True)
    first_name = models.CharField(_('first name'), max_length = 32, blank = True)
    last_name = models.CharField(_('last name'), max_length = 32, blank = True)
    image_path = models.ImageField(default = 'default.jpg', upload_to = 'user_avatars/', blank = True)
    is_active = models.BooleanField(('active'), default = True)
    is_staff = models.BooleanField(('staff'), default=False) 

    objects = UserManager()

    USERNAME_FIELD = 'username'
    REQUIRED_FIELDS = []

    class Meta:
        verbose_name = _('user')
        verbose_name_plural = _('users')

    def get_full_name(self):
        full_name = '%s %s' % (self.first_name, self.last_name)
        return full_name.strip()

    def get_short_name(self):
        return self.first_name
