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

INSERT INTO leitura(Vazao, Temp, EstadoComp, DutyCycle, Data, IDAerador) VALUES ('$hora',$vazao,$temp,$estadoComp,$dc,'$data',$IdAerador)
INSERT INTO aerador(Nome, IDUsuario, DataManut, DataInsta, MarcaComp, CapComp, Nboias, MacAddress,freq) VALUES ("Aerador Xaxim","1","21/10/21","20/10/21","WEG","5","4","00-0C-23-D3-89-A1","941654258");
alter table aerador AUTO_INCREMENT = 1;

//Seleciona última leitura de cada ID
SELECT * FROM leitura WHERE IDLeitura IN (SELECT max(IDLeitura) FROM leitura GROUP BY IDAerador);

UPDATE aerador SET DutyCycle= ('{"DC":[{"1:00":"0.2"}, {"2:00":"0.3"}]}');


      Curl

curl -i -X POST -H 'Content-Type: application/json' -d '{"address":"Sunset Boulevard"}' http://localhost:8000/_php/registraMedicoes.php

curl -i -X POST -H 'Content-Type: application/json' -d '{"hora":"7e2","vazao":"2","temp":"35","estadoComp":"1","dutyCycle":"1","data":"22dejaneiro","IdAerador":"3"}' http://localhost:8000/_php/registraMedicoes.php

curl -i -X POST -H 'Content-Type: application/json' -d '["24:0A:C4:FA:74:A8"]'  http://localhost:8000/_php/lerConfigAerador.php


curl -i -X POST -H 'Content-Type: application/json' -d '[{"freq":941654258,"DC":0.600000024,"MAC":"24:0A:C4:FA:7D:60","State":true,"rssI":-61,"temp":21.12000084}]' http://localhost:8000/_php/registraMedicoes.php

sudo rm -r /var/www/html*
sudo cp -R ~/Documents/ProjetoPiIII/* /var/www/html
