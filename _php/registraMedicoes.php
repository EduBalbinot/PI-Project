
<?php
echo "aa";
include 'acess.php';
//Código de conexão ao BD
//include "conexao.php";
$data = file_get_contents("php://input");

if (strtoupper($_SERVER['REQUEST_METHOD']) != 'POST') {
  // throw new Exception('Only POST requests are allowed');
  echo "erro1";
}

$content_type = isset($_SERVER['CONTENT_TYPE']) ? $_SERVER['CONTENT_TYPE'] : '';
if (stripos($content_type, 'application/json') === false) {
  echo "erro2";
  throw new Exception('Content-Type must be application/json');
}
else
{
// Read the input stream
$body = file_get_contents("php://input");

// Decode the JSON object
$object = json_decode($body, true);
$obj = json_decode($body);

// Throw an exception if decoding failed
if (!is_array($object)) {
  echo "erro3";
  throw new Exception('Failed to decode JSON object');
  }
}

$hora = $obj->hora;
$vazao = $obj->vazao;
$temp = $obj->temp;
$estadoComp = $obj->estadoComp;
$dc = $obj->dutyCycle;
$data = $obj->data;
$IdAerador = $obj->IdAerador;

$conn = con();

echo $obj->Aeradores[0]->hora;
echo $obj->Aeradores[1]->hora;

// echo $hora,$vazao,$temp,$estadoComp,$dc,$data,$IdAerador;

$sql = "INSERT INTO leitura(Hora, Vazao, Temp, EstadoComp, DutyCycle, Data, IDAerador)
                  VALUES ('$hora',$vazao,$temp,$estadoComp,$dc,'$data',$IdAerador)" ; // Insere no DB
mysqli_query($conn, $sql);
$conn->close();

//TERMINAL COMMAND
// sudo tail -f /var/log/apache2/access.log
// curl -i -X POST -H 'Content-Type: application/json' -d '{"address":"Sunset Boulevard"}' http://192.168.1.158/registraMedicoes.php
?>
