from django.db import models
from django.utils import timezone
from django.conf import settings

class Posts(models.Model):
    postedAt = models.DateTimeField(default = timezone.now, editable = False)
    latitude = models.DecimalField(max_digits = 9, decimal_places = 6)
    longitude = models.DecimalField(max_digits = 9, decimal_places = 6)
    body = models.TextField()
    user = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete = models.CASCADE)

    def __str__(self):
        return f"Post by {self.author}"

class Comment(models.Model):
    body = models.CharField(max_length = 255)
    user = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete = models.CASCADE)
    post = models.ForeignKey(Posts, on_delete = models.CASCADE)

class Category(models.Model):
    tag = models.CharField(max_length = 32,primary_key = True, unique = True)

class CategoryList(models.Model):
    postId = models.ForeignKey(Posts, on_delete = models.CASCADE)
    tag = models.ForeignKey(Category, on_delete = models.CASCADE)

class Likes(models.Model):
    postId = models.ForeignKey(Posts, on_delete = models.CASCADE)
    user = models.ForeignKey(settings.AUTH_USER_MODEL, on_delete = models.CASCADE)
    createdAt = models.DateTimeField(default = timezone.now, editable = False)
