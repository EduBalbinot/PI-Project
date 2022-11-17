<?php
function con(){
  $servername = "localhost";
  $username = "root";
  $password = "password";
  $dbname = "usuario";
  // Cria conexão
  $conn = new mysqli($servername, $username, $password, $dbname);
  // Verifica conexão
  if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
  }
  return $conn;
}

function sessao() {
  session_start();
  return $_SESSION["userID"];
}

function hiddenID(){
  $id=sessao();
  echo "<input type=\"hidden\" name=\"aa\" id=\"aa\" value=\""
   .$id. "\"/>";
   return $id;
}

function sessaoAerador() {
  session_start();
  return $_SESSION["IDAerador"];
}

function switchOn($id) {
  $conn = con();
  // Apresenta os dados do DB
  $sql = "SELECT ligado FROM aerador WHERE IdAerador=$id";
  $result = $conn->query($sql);
  $a=$result->fetch_assoc();
  echo $a["ligado"];
}

function tabela($sessao){
  $conn = con();
  // Apresenta os dados do DB
  $sql = "SELECT IDAerador,Nome,NBoias,DutyCycle,CapComp,MarcaComp,freq,MacAddress,DATE_FORMAT(DataInsta, '%d/%m/%Y'),DATE_FORMAT(DataManut, '%d/%m/%Y'),ligado FROM aerador WHERE IDUsuario=$sessao";
  $result = $conn->query($sql);
  if ($result->num_rows > 0) {
    echo "<section class=\"cards\">";
    while($row = $result->fetch_assoc()) {
      $ligado=$row["ligado"];
      if($ligado) $color="green";
      else $color="red";
      echo "<article><button id=\"".$row["IDAerador"]."\" onclick=botaoAerador(this.id) class=\"card ".$color. "\">
      <h3>" .  $row["Nome"]   .                                "</h3>
      <p>ID:" .  $row["IDAerador"]   .                        "</p>
      <p>Número de bóias: " .  $row["NBoias"]   .       " bóias</p>
      <p>Capacidade: " .  $row["CapComp"]   .               "m³</p>
      <p>Marca: " .  $row["MarcaComp"]   .                    "</p>
      <p>Frequencia: " .  $row["freq"]   .              "</p>
      <p>MAC: " .  $row["MacAddress"]   .                     "</p>
      <p>Data Instalação: " .  $row["DATE_FORMAT(DataInsta, '%d/%m/%Y')"]   .          "</p>
      <p>Data Última Manutenção:</p><p> " .  $row["DATE_FORMAT(DataManut, '%d/%m/%Y')"]   .   "</p>
      </button></article>";
    }
    echo "</section>";
  } else {
    echo "0 results";
  }
$conn->close();
}

function tabelaLeitura($IDAerador){
  $conn = con();
  // Apresenta os dados do DB
  $day= date('d');
  $mes= date('m');
  $ano= date('Y');
  $sql = "SELECT IDAerador,Nome,NBoias,DutyCycle,CapComp,MarcaComp,freq,MacAddress,DATE_FORMAT(DataInsta, '%d/%m/%Y'),DATE_FORMAT(DataManut, '%d/%m/%Y'),ligado FROM aerador WHERE IDAerador=$IDAerador";
  $result = $conn->query($sql);
  if ($result->num_rows > 0) {
    $info=$result->fetch_assoc();
  }
  $sql = "SELECT Temp,Vazao,Data,DutyCycle,DATE_FORMAT(Data, '%d/%m/%Y %H:%i:%s'),rssI,EstadoComp FROM leitura WHERE IDLeitura IN (SELECT max(IDLeitura) FROM leitura GROUP BY IDAerador) AND IDAerador=$IDAerador";
  $result = $conn->query($sql);
  if ($result->num_rows > 0) {
    $leitura=$result->fetch_assoc();
  }
  $sql = "SELECT Temp,Vazao,Data FROM leitura WHERE IDAerador=$IDAerador AND DAY(Data)=$day AND MONTH(Data)=$mes AND YEAR(Data)=$ano";
  $result = $conn->query($sql);
  if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
      $temp[]=$row["Temp"];
      $vazao[]=$row["Vazao"];
      $data[]=$row["Data"];
    }
  }
$conn->close();
return array($data,$vazao,$temp,$info,$leitura);
}

function tabelaMenu($sessao){
  $conn = con();
  // Apresenta os dados do DB
  $sql = "SELECT Nome,IDAerador FROM aerador WHERE IDUsuario=$sessao";
  $result = $conn->query($sql);
  if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
      echo "<button class=\"menuButton\" id=\"".$row["IDAerador"]."\" onclick=botaoAerador(this.id)>".$row["Nome"]."";
    }
  } else {
    echo "0 results";
  }
$conn->close();
}
?>
