/*

Projeto para monitoramento de pequenos animais - conexão entre ESP e planilha Google
Autor: Maria Alice de Melo Sousa & Josiel Patrício 
Instituição: UFRN 
Orientador: Marconi Camara

*/


#include <ESP8266WiFi.h>
#include <WiFiManager.h>

// RTC
#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 RTC;

//Conexão planilha Google
WiFiClientSecure client;//Cria um cliente seguro (para ter acesso ao HTTPS)
String strtosend = "GET /forms/d/e/1FAIpQLSebzLiNa-XS28xq6y-Nkp1ZtpKGvipdHWweiBNxZlA7pVn02Q/formResponse?ifq&entry.1212020823={a}&entry.149109020={r}";
//Essa String sera uma auxiliar contendo o link utilizado pelo GET, para nao precisar ficar re-escrevendo toda hora
 


// timeToSend 5 segundos para teste e 5 minutos {5(min) * 60(s) *1000 (milissegundos) = (300000 millissegundos)} para aplicação real
unsigned long lastTimeLoop = 0, timeToSend = 1000; 

void setup()
{
    //Inicia a comunicacao serial
    Serial.begin(9600);
    
    // Para cadastro do WiFi
    WiFiManager wm;
    wm.autoConnect("teste de Maria Alice"); 
    
    //RTC
    Wire.begin();
    RTC.begin();
    if (! RTC.isrunning()) {
        Serial.println("RTC is NOT running!");
        // following line sets the RTC to the date & time this sketch was compiled
        RTC.adjust(DateTime(__DATE__, __TIME__));
    }


}
 
 
void loop()
{

    if (millis() - lastTimeLoop >= timeToSend) {
    DateTime now = RTC.now(); 
    String datetime = "";
    
    datetime +=now.year();
    datetime +="/";
    datetime +=now.month();
    datetime +="/";
    datetime +=now.day();
    
    datetime +=" ";

    datetime +=now.hour();
    datetime +=":";
    datetime +=now.minute();
    datetime +=":";
    datetime +=now.second();
    
   
        
        if (client.connect("docs.google.com", 443) == 1)//Tenta se conectar ao servidor do Google docs na porta 443 (HTTPS)
        {
            
            String toSend = strtosend;//Atribuimos a String auxiliar na nova String que sera enviada
            toSend.replace("{a}", String(random(0, 501)));//Adicionamos um valor aleatorio
            toSend.replace("{r}", datetime);
            toSend += "&submit=Submit HTTP/1.1";//Completamos o metodo GET para nosso formulario.
            Serial.println(toSend);

            client.println(toSend);//Enviamos o GET ao servidor-
            client.println("Host: docs.google.com");//-
            client.println();//-
            client.stop();//Encerramos a conexao com o servidor
            Serial.println("Dados enviados.");//Mostra no monitor que foi enviado
        }
        else
        {
            Serial.println("Erro ao se conectar");//Se nao for possivel conectar no servidor, ira avisar no monitor.
        }
    lastTimeLoop = millis();
    }
}

