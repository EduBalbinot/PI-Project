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

function tabela($sessao){
  $conn = con();
  // Apresenta os dados do DB
  $sql = "SELECT * FROM aerador WHERE IDUsuario=$sessao";
  $result = $conn->query($sql);
  if ($result->num_rows > 0) {
    // output data of each row
    echo "<table id=\"tabelaAcesso\">
            <tr>
              <th>Id</th>
              <th>Nome</th>
              <th>Boias</th>
              <th>Capacidade</th>
              <th>Marca</th>
              <th>Data Insta.</th>
              <th>Data Manut.</th>
              <th>MAC</th>
              <th>Deletar</th>
              <th>Editar</th>
            </tr>";

    while($row = $result->fetch_assoc()) {
      echo "<tr id=\"aerador".$row["IDAerador"].      "\">
           <td>" .  $row["IDAerador"]   .             "</td>
           <td><button id=\"".$row["IDAerador"]."\" onclick=botaoAerador(this.id)>".$row["Nome"]."</td>
           <td>" .  $row["NBoias"]      .             "</td>
           <td>" .  $row["CapComp"]     .             "</td>
           <td>" .  $row["MarcaComp"]   .             "</td>
           <td>" .  $row["DataInsta"]   .             "</td>
           <td>" .  $row["DataManut"]   .             "</td>
           <td>" .  $row["MacAddress"]  .             "</td>
           <td><button id=\"".$row["IDAerador"]."\" onclick=botaoDeleta(this.id)>Deletar</td>
           <td><button id=\"".$row["IDAerador"]."\" onclick=botaoEdita(this.id)>Editar</td>
           </tr>";
    }
    echo "</table>";
  } else {
    echo "0 results";
  }
$conn->close();
}

function tabelaLeitura($IDAerador){
  $conn = con();
  // Apresenta os dados do DB
  $sql = "SELECT * FROM leitura WHERE IDAerador=$IDAerador";
  $result = $conn->query($sql);
  if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
      $temp[]=$row["Temp"];
      $vazao[]=$row["Vazao"];
      $hora[]=$row["Hora"];
    }
  }
$conn->close();
return array($hora,$vazao,$temp);
}

function tabelaMenu($sessao){
  $conn = con();
  // Apresenta os dados do DB
  $sql = "SELECT Nome,IDAerador FROM aerador WHERE IDUsuario=$sessao";
  $result = $conn->query($sql);
  if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
      echo "<a><button class=\"menuButton\" id=\"".$row["IDAerador"]."\" onclick=botaoAerador(this.id)>".$row["Nome"]."</a>";
    }
  } else {
    echo "0 results";
  }
$conn->close();
}
?>
