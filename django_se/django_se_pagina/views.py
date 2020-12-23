from django.shortcuts import render
from django.template import loader

from .models import Dht11
from django.http import HttpResponse

def index(request):
    return HttpResponse("Hello, world. You're at the polls index.")

def index2(request):
    dht11 = Dht11.objects.order_by('-data')[:5]
    template = loader.get_template('index.html')
    context = {
        'teste': dht11,
    }
    return HttpResponse(template.render(context, request))
    
def pie_chart(request):
    datas = []
    temperatura = []
    humidade = []

    queryset = Dht11.objects.order_by('data')[:5]
    for city in queryset:
        datas.append(city.data.strftime("%d/%m/%Y %H:%M:%S"))
        humidade.append(city.humidade)
        temperatura.append(city.temperatura)

    template = loader.get_template('pie_chart.html')
    context = {
        'datas': datas,
        'temperatura': temperatura,
        'humidade': humidade,
    }

    return HttpResponse(template.render(context, request))