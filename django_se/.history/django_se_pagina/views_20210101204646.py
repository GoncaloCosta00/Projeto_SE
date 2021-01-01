from django.shortcuts import render
from django.template import loader
from datetime import datetime, timedelta

from .models import Dht11, Caixote, Mq135
from django.http import HttpResponse,JsonResponse
from django.views.decorators.csrf import csrf_exempt #ignorar os tokens, etc

#json encoder
from django.core.serializers.json import DjangoJSONEncoder
from json import dumps as json_dumps

def index(request):
    return HttpResponse("Hello, world. You're at the polls index.")

def index2(request):
    dht11 = Dht11.objects.order_by('-data')[:5]
    template = loader.get_template('index.html')
    context = {
        'teste': dht11,
    }
    return HttpResponse(template.render(context, request))
    
def dht11_chart(request):
    datas = []
    temperatura = []
    humidade = []

    queryset = Dht11.objects.order_by('data')
    for dados in queryset:
        datas.append(dados.data.strftime("%d/%m/%Y %H:%M:%S"))
        humidade.append(dados.humidade)
        temperatura.append(dados.temperatura)

    template = loader.get_template('dht11_chart.html')
    context = {
        'datas': datas,
        'temperatura': temperatura,
        'humidade': humidade,
    }

    return HttpResponse(template.render(context, request))
    
def mq135_chart(request):
    datas = []
    co = []
    co2 = []
    alcohol = []
    nh4 = []

    last_month = datetime.now() - timedelta(days=15)
    queryset = Mq135.objects.filter(data__gte=datetime.date(2021,1,1)).order_by('data')
    for dados in queryset:
        datas.append(dados.data.strftime("%d/%m/%Y %H:%M:%S"))
        co.append(dados.co)
        co2.append(dados.co2)
        alcohol.append(dados.alcohol)
        nh4.append(dados.nh4)

    template = loader.get_template('mq135_chart.html')
    context = {
        'datas': datas,
        'co': co,
        'co2': co2,
        'alcohol': alcohol,
        'nh4': nh4,
    }

    return HttpResponse(template.render(context, request))
    
def caixote_chart(request):
    datas = []
    utilizacao = []

    queryset = Caixote.objects.order_by('data')
    for dados in queryset:
        datas.append(dados.data.strftime("%d/%m/%Y %H:%M:%S"))
        utilizacao.append(dados.perc_utilizacao)

    template = loader.get_template('caixote_chart.html')
    context = {
        'datas': datas,
        'utilizacao': utilizacao,
    }

    return HttpResponse(template.render(context, request))
    
@csrf_exempt
def save_info(request):
    if request.method == "POST":

        if "temperatura" in request.GET:
            if "humidade" in request.GET:
                print("temperatura: " + request.GET["temperatura"])
                print("humidade: " + request.GET["humidade"])
                Dht11.objects.create(temperatura = request.GET["temperatura"], humidade = request.GET["humidade"])
        if "co" in request.GET:
            if "alcohol" in request.GET:
                if "co2" in request.GET:
                    if "nh4" in request.GET:
                        print("co: " + request.GET["co"])
                        print("alcohol: " + request.GET["alcohol"])
                        print("co2: " + request.GET["co2"])
                        print("nh4: " + request.GET["nh4"])
                        Mq135.objects.create(co = request.GET["co"], alcohol = request.GET["alcohol"], co2 = request.GET["co2"], nh4 = request.GET["nh4"])
        if "perc_utilizacao" in request.GET:
            print("perc_utilizacao: " + request.GET["perc_utilizacao"])
            Caixote.objects.create(perc_utilizacao = request.GET["perc_utilizacao"])
            
        return HttpResponse("dados guardados!")
    
    return HttpResponse("")
