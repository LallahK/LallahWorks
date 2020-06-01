from django.db import models
from django.utils import timezone
from django.conf import settings

class Groups(models.Model):
    name = models.CharField(max_length = 64)
    createdAt = models.DateTimeField(default = timezone.now, editable = False)
    
    def __str__(self):
        return self.name

class Messages(models.Model):
    sentAt = models.DateTimeField(default = timezone.now, editable = False)
    body = models.TextField()
    sender = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete = models.CASCADE)
    group = models.ForeignKey(Groups, on_delete = models.CASCADE)
    
    def __str__(self):
        return self.sender + " - " + self.group + " " + self.sentAt

class Admins(models.Model):
    user = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete = models.CASCADE)
    group = models.ForeignKey(Groups, on_delete = models.CASCADE)

    def __str__(self):
        return self.user + " - " + self.group

class Members(models.Model):
    user = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete = models.CASCADE)
    group = models.ForeignKey(Groups, on_delete = models.CASCADE)

    def __str__(self):
        return self.user + " - " + self.group
