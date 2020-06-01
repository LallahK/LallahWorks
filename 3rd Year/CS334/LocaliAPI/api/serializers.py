# This file will contain all serializers for our Django models.

from Profile.models import *
from rest_framework import serializers
class UserSerializer(serializers.ModelSerializer):
    class Meta:
        model = User
        fields = '__all__'