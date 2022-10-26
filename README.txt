    Protocolo de Comunicação

inicio comunicação

1) LoRaM mestre requisita dados dos aeradores
				envia macAdress dos aeradores
2)Webserver reponde
				passa os endereços mac e vê se tem um novo
				lê o array dos DC
				envia dados (Json)
3)LoRaM envia os parametros para cada LoRaE
				envia dados (Json)
4)LoRaE responde a média da medição
				dadosLeitura (Json)
5)LoraM escreve as leituras no DB leituras

fim da comunicação


    Comandos Terminal

sudo tail -f /var/log/apache2/access.log

ip addr show
alter table aerador AUTO_INCREMENT = 1;

      SQL

INSERT INTO leitura(Hora, Vazao, Temp, EstadoComp, DutyCycle, Data, IDAerador) VALUES ('$hora',$vazao,$temp,$estadoComp,$dc,'$data',$IdAerador)
INSERT INTO aerador(Nome, IDUsuario, DataManut, DataInsta, MarcaComp, CapComp, Nboias, MacAddress,freq) VALUES ("Aerador Xaxim","1","21/10/21","20/10/21","WEG","5","4","00-0C-23-D3-89-A1","941654258");
alter table aerador AUTO_INCREMENT = 1;

UPDATE aerador SET DutyCycle= ('{"DC":[{"1:00":"0.2"}, {"2:00":"0.3"}]}');


      Curl

curl -i -X POST -H 'Content-Type: application/json' -d '{"address":"Sunset Boulevard"}' http://localhost:8000/_php/registraMedicoes.php

curl -i -X POST -H 'Content-Type: application/json' -d '{"hora":"7e2","vazao":"2","temp":"35","estadoComp":"1","dutyCycle":"1","data":"22dejaneiro","IdAerador":"3"}' http://localhost:8000/_php/registraMedicoes.php

curl -i -X POST -H 'Content-Type: application/json' -d '["00-0C-23-D3-89-A1","34-0F-12-25-9A-D1","62-B1-00-34-99-E4"]'  http://localhost:8000/_php/lerConfigAerador.php


curl -i -X POST -H 'Content-Type: application/json' -d '{"Aeradores":[{"hora":"7e2","vazao":"2","temp":"35","estadoComp":"1","dutyCycle":"1","data":"22dejaneiro","IdAerador":"3"},
{"hora":"7e3","vazao":"2","temp":"35","estadoComp":"1","dutyCycle":"1","data":"22dejaneiro","IdAerador":"3"}]}' http://localhost:8000/_php/registraMedicoes.php
