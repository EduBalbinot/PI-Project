
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
for ($x = 0; $x < sizeof($obj); $x++) {
  // $vazao      = $obj[$x]->vazao;
  $temp       = $obj[$x]->temp;
  if($temp>-20){ //verifica se a leitura foi bem sucedida, leitura má sucedida é $temp=-125
    $vazao      = 30;
    $estadoComp = $obj[$x]->State;
    $dc         = $obj[$x]->DC;
    $MAC        = $obj[$x]->MAC;
    $freq       = $obj[$x]->freq;
    $rssI       = $obj[$x]->rssI;

    $sql = "SELECT IDAerador FROM aerador WHERE MACMestre ='$MAC'";
    $result = $conn->query($sql);
    $info = $result->fetch_assoc();
    $IdAerador=$info["IDAerador"];

    $sql = "INSERT INTO leitura(Vazao, Temp, EstadoComp, DutyCycle, IDAerador, rssI)
                      VALUES ($vazao,$temp,$estadoComp,$dc,$IdAerador,$rssI)" ; // Insere no DB
    mysqli_query($conn, $sql);
    print "Leitura armazenada do aerador Id: $IdAerador";
  }
  else {
    print "Falha na leitura da temperatura";
  }
}
$conn->close();

?>
