# Generated by Django 2.1 on 2020-05-15 21:02

import Profile.UserManager
from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
        ('auth', '0009_alter_user_last_name_max_length'),
    ]

    operations = [
        migrations.CreateModel(
            name='User',
            fields=[
                ('password', models.CharField(max_length=128, verbose_name='password')),
                ('last_login', models.DateTimeField(blank=True, null=True, verbose_name='last login')),
                ('is_superuser', models.BooleanField(default=False, help_text='Designates that this user has all permissions without explicitly assigning them.', verbose_name='superuser status')),
                ('username', models.CharField(db_index=True, editable=False, max_length=32, primary_key=True, serialize=False, unique=True)),
                ('email', models.EmailField(max_length=254, verbose_name='email address')),
                ('date_joined', models.DateTimeField(auto_now_add=True, verbose_name='date joined')),
                ('first_name', models.CharField(blank=True, max_length=32, verbose_name='first name')),
                ('last_name', models.CharField(blank=True, max_length=32, verbose_name='last name')),
                ('image_path', models.ImageField(blank=True, default='default.jpg', upload_to='user_avatars/')),
                ('is_active', models.BooleanField(default=True, verbose_name='active')),
                ('groups', models.ManyToManyField(blank=True, help_text='The groups this user belongs to. A user will get all permissions granted to each of their groups.', related_name='user_set', related_query_name='user', to='auth.Group', verbose_name='groups')),
                ('user_permissions', models.ManyToManyField(blank=True, help_text='Specific permissions for this user.', related_name='user_set', related_query_name='user', to='auth.Permission', verbose_name='user permissions')),
            ],
            options={
                'verbose_name': 'user',
                'verbose_name_plural': 'users',
            },
            managers=[
                ('objects', Profile.UserManager.UserManager()),
            ],
        ),
    ]
