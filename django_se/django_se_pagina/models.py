# This is an auto-generated Django model module.
# You'll have to do the following manually to clean this up:
#   * Rearrange models' order
#   * Make sure each model has one field with primary_key=True
#   * Make sure each ForeignKey and OneToOneField has `on_delete` set to the desired behavior
#   * Remove `managed = False` lines if you wish to allow Django to create, modify, and delete the table
# Feel free to rename the models, but don't rename db_table values or field names.
from django.db import models

class Caixote(models.Model):
    perc_utilizacao = models.FloatField(blank=True, null=True)
    id = models.AutoField(primary_key=True)
    data = models.DateTimeField(auto_now=True, null=True)

    class Meta:
        #managed = False
        db_table = 'caixote'


class Dht11(models.Model):
    temperatura = models.FloatField(blank=True, null=True)
    humidade = models.FloatField(blank=True, null=True)
    data = models.DateTimeField(auto_now=True, null=True)
    id = models.AutoField(primary_key=True) 
    #autofield -> identity
    #auto_now coloca a hora atual como default

    class Meta:
        #managed = False
        db_table = 'dht11'

class Mq135(models.Model):
    alcohol = models.FloatField(blank=True, null=True)
    co2 = models.FloatField(blank=True, null=True)
    nh4 = models.FloatField(blank=True, null=True)
    co = models.FloatField(blank=True, null=True)
    data = models.DateTimeField(auto_now=True, null=True)
    id = models.AutoField(primary_key=True)

    class Meta:
        #managed = False
        db_table = 'mq135'
