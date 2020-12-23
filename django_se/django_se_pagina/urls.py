from django.urls import path
from . import views #import da view a usar

urlpatterns = [
    path('', views.index, name='index'), #configuração do caminho e parte da view a usar
    path('2/', views.index2, name='index'),
    path('pie_chart/', views.pie_chart, name='index'),
]
