from django.urls import path
from django.conf.urls import include
from rest_framework.routers import DefaultRouter
from api.views import *

router = DefaultRouter()

urlpatterns = [
    path('users/', UserListView.as_view(), name='users_list'),
    path('', include(router.urls))
]