from django.db import models
from django.conf import settings

class Friends(models.Model):
    user = models.ForeignKey(settings.AUTH_USER_MODEL, related_name = 'user', on_delete = models.CASCADE)
    friend = models.ForeignKey(settings.AUTH_USER_MODEL, related_name = 'friend', on_delete = models.CASCADE)
