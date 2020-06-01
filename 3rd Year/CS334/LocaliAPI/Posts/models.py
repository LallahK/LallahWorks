from django.db import models
from django.utils import timezone
from django.conf import settings
from django.contrib.gis.db import models as geomodels

class Category(models.Model):
    tag = models.CharField(max_length = 32, unique = True)

    class Meta:
        verbose_name = 'Category'
        verbose_name_plural = 'Categories'

class Post(models.Model):
    posted_at = models.DateTimeField(default = timezone.now, editable = False)
    latitude = models.DecimalField(max_digits = 9, decimal_places = 6)
    longitude = models.DecimalField(max_digits = 9, decimal_places = 6)
    body = models.TextField()
    user = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete = models.CASCADE)
    categories = models.ManyToManyField(Category, blank=True)
    private = models.BooleanField(('private'), default = False);
    
    def __str__(self):
        return f"Post by {self.user}"

class Comment(models.Model):
    body = models.CharField(max_length = 255)
    user = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete = models.CASCADE)
    post = models.ForeignKey(Post, on_delete = models.CASCADE)

class Like(models.Model):
    post = models.ForeignKey(Post, on_delete = models.CASCADE)
    user = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete = models.CASCADE)
    created_at = models.DateTimeField(default = timezone.now, editable = False)
