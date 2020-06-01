# Generated by Django 2.1 on 2020-05-15 21:02

from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    initial = True

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
        ('Groups', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='messages',
            name='sender',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to=settings.AUTH_USER_MODEL),
        ),
        migrations.AddField(
            model_name='members',
            name='group',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='Groups.Groups'),
        ),
        migrations.AddField(
            model_name='members',
            name='user',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to=settings.AUTH_USER_MODEL),
        ),
        migrations.AddField(
            model_name='admins',
            name='group',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='Groups.Groups'),
        ),
        migrations.AddField(
            model_name='admins',
            name='user',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to=settings.AUTH_USER_MODEL),
        ),
    ]