<?php
if ($_POST['nomeCadastro']) {

  include 'acess.php';
  $conn = con();
  $senha = $_POST['senha'];
  $nome = $_POST['nomeCadastro'];
  $email = $_POST['email'];

  $sql = "INSERT INTO usuario(Nome, senha, email) VALUES ('$nome','$senha','$email')"; // Insere no DB
    mysqli_query($conn, $sql);
    $conn->close();
}
?>
