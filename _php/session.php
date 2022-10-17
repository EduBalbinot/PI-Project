<?php
if ($_POST['nomeCadastro'] && $_POST['senha']) {

  include 'acess.php';
  $conn = con();
  $name =  $_POST['nomeCadastro'];           // Pega os dados do formulário
  $senha = $_POST['senha'];

  $sql = "SELECT * FROM usuario WHERE Nome='$name'";
  $result = $conn->query($sql);
    // if(mysqli_query($conn, $sql)){
    //   echo "done";                                  // Responde sucesso
    // }
    // header( "Location: {$_SERVER['REQUEST_URI']}", true, 303 );  // Método PRG 303-> resposta padrão pra form
    // exit();
  $info = $result->fetch_assoc();
  $userSenha=$info["senha"];
   if($userSenha == $senha)
  {
    session_start();
    $_SESSION["userID"]=$info["IDUsuario"];
    echo json_encode($_SESSION["userID"]);
  }
  else
  {
  echo json_encode(-1);
  }
  $conn->close();
}
else
{
    echo json_encode(-1);
}
?>
