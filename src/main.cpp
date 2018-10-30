/*



Projeto para monitoramento de pequenos animais - conexão entre ESP e planilha Google
Autor: Maria Alice de Melo Sousa
Instituição: UFRN 
Orientador: Marconi Camara



*/


#include <ESP8266WiFi.h>
 
WiFiClientSecure client;//Cria um cliente seguro (para ter acesso ao HTTPS)
String textFix = "GET /forms/d/e/1FAIpQLSebzLiNa-XS28xq6y-Nkp1ZtpKGvipdHWweiBNxZlA7pVn02Q/formResponse?ifq&entry.1212020823=";
//Essa String sera uma auxiliar contendo o link utilizado pelo GET, para nao precisar ficar re-escrevendo toda hora
 
 
void setup()
{
    Serial.begin(9600);//Inicia a comunicacao serial
    WiFi.mode(WIFI_STA);//Habilita o modo estaçao
    WiFi.begin("saiot", "u2345678");//Conecta na rede
    
    delay(2000);//Espera um tempo para se conectar no WiFi
 
}
 
 
void loop()
{




    if (client.connect("docs.google.com", 443) == 1)//Tenta se conectar ao servidor do Google docs na porta 443 (HTTPS)
    {
        String toSend = textFix;//Atribuimos a String auxiliar na nova String que sera enviada
        toSend += random(0, 501);//Adicionamos um valor aleatorio
        toSend += "&submit=Submit HTTP/1.1";//Completamos o metodo GET para nosso formulario.
 
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
 
    delay(5000);
}