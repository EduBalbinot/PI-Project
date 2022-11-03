
<?php
//Biblioteca de conexão ao BD
include 'acess.php';

//Confere se é método POST
if (strtoupper($_SERVER['REQUEST_METHOD']) != 'POST') {
  throw new Exception('Only POST requests are allowed');
}

//Confere se o formato de arquivo é Json
$content_type = isset($_SERVER['CONTENT_TYPE']) ? $_SERVER['CONTENT_TYPE'] : '';
if (stripos($content_type, 'application/json') === false) {
  throw new Exception('Content-Type must be application/json');
}
else
{

//Le o arquivo recebido
$body = file_get_contents("php://input");

// Decodifica o objeto Json
$object = json_decode($body, true);
$obj = json_decode($body);

// Avisa se a decodificação falhar
if (!is_array($object)) {
  throw new Exception('Failed to decode JSON object');
  }
}

$conn = con();
$aerador=$obj->Aeradores;
for ($x = 0; $x < sizeof($obj->Aeradores); $x++) {
  echo "Aerador $x:";
  $hora       = $aerador[$x]->hora;
  $vazao      = $aerador[$x]->vazao;
  $temp       = $aerador[$x]->temp;
  $estadoComp = $aerador[$x]->estadoComp;
  $dc         = $aerador[$x]->dutyCycle;
  $data       = $aerador[$x]->data;
  $IdAerador  = $aerador[$x]->IdAerador;
  print "hora: $hora, vazao: $vazao, temp: $temp, compressor: $estadoComp, DC: $dc, data: $data, Id: $IdAerador\n";
 // $sql = "INSERT INTO leitura(Hora, Vazao, Temp, EstadoComp, DutyCycle, Data, IDAerador)
 //                   VALUES ('$hora',$vazao,$temp,$estadoComp,$dc,'$data',$IdAerador)" ; // Insere no DB
 // mysqli_query($conn, $sql);
}
$conn->close();
?>
