from django.db import models
from django.utils import timezone
from django.conf import settings
from django.contrib.auth.models import Posts

class Groups(models.Model):
    name = models.CharField(max_length = 64)
    created_at = models.DateTimeField(default = timezone.now, editable = False)
    admins = models.ManyToManyField(settings.AUTH_USER_MODEL, related_name = 'group_admins')
    members = models.ManyToManyField(settings.AUTH_USER_MODEL, related_name = 'group_members')
    
    def __str__(self):
        return self.name

class Posts(models.Model):
    post = models.ForeignKey(Posts, on_delete = models.CASCADE)
    group = models.ForeignKey(Groups, on_delete = models.CASCADE)
    
    def __str__(self):
        return self.sender + " - " + self.group + " " + self.sentAt
