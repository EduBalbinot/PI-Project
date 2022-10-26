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
$object = json_decode($body, true); //Decodifica para array
$obj = json_decode($body);            //Decodifica para objeto
// Avisa se a decodificação falhar
if (!is_array($object)) {
  throw new Exception('Failed to decode JSON object');
  }
}

$arr=[];
$ARR=[];
$new=[];
$DC=[];

$conn = con();

$nbrDCarray=12;//numero de DC no array
date_default_timezone_set('america/sao_paulo');
$DCIndex = (date('H')*60+date('i'))/((24*60)/$nbrDCarray);

foreach ($object as &$value) {
  // Apresenta os dados do DB
  $sql = "SELECT * FROM aerador WHERE MacAddress= '$value'";
  $result = $conn->query($sql);

  if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
      $DC=json_decode($row["DutyCycle"]);
      array_push($ARR,array("Mac"=>$row["MacAddress"], "ID"=>$row["IDAerador"], "freq"=>$row["freq"],"DC"=>$DC[$DCIndex]));
    }
  } else {
      print "Mac Inválido: $value\n ";
  }
}

//adiciona as frequencias dos aeradores sem endereços MAC
$sql = "SELECT * FROM aerador WHERE MacAddress IS NULL";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
  while($row = $result->fetch_assoc()) {
    $DC=json_decode($row["DutyCycle"]);
    array_push($new,array("ID"=>$row["IDAerador"], "freq"=>$row["freq"], "DC"=>$DC[$DCIndex]));
  }
} else {
  print "Sem nada novo";
}

  // $ARR["new"]=$new;
  $arr[0]=$ARR;
  $arr[1]=$new;

  print_r($arr);
  $conn->close();
?>
