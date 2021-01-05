from django.urls import path
from . import views #import da view a usar

urlpatterns = [
    path('', views.index, name='index'), #configuração do caminho e parte da view a usar
    path('save_info', views.save_info),
    path('dht11', views.dht11_chart, name='index'),
    path('mq135', views.mq135_chart, name='index'),
    path('caixote', views.caixote_chart, name='index'),
    
]
