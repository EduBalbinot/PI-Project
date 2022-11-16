<?php

  include 'acess.php';
  $conn = con();

  $body = file_get_contents("php://input");
  $IDAerador=sessaoAerador();
  $i=2;
  $dc=json_encode($_POST);

  $sql = "UPDATE aerador SET DutyCycle=('$dc') WHERE IDAerador=$IDAerador"; // Insere no DB
    mysqli_query($conn, $sql);
    $conn->close();
?>
