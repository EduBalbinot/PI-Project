<?php
if ($_POST['nomeCadastro']) {

  include 'acess.php';
  $conn = con();
  $mac = $_POST['macAddress'];
  $MarcaComp = $_POST['marcaComp'];
  $DataInsta = $_POST['dataInsta'];
  $DataManut = $_POST['dataManut'];
  $Nome = $_POST['nomeCadastro'];
  $id = $_POST['idCadastro'];
  $CapComp = $_POST['capacidade'];
  $NBoias = $_POST['nBoias'];

  if($NBoias=="") $NBoias="NULL";
  if($CapComp=="") $CapComp="NULL";

  $IDUser = sessao();

  if($id>=0) $sql = "UPDATE aerador SET MacAddress='$mac', NBoias=$NBoias, CapComp=$CapComp, MarcaComp='$MarcaComp',
                      DataInsta='$DataInsta', DataManut='$DataManut',Nome='$Nome' WHERE IDAerador='$id'"; // Modifica no DB
  else $sql = "INSERT INTO aerador(MacAddress, NBoias,CapComp, MarcaComp, DataInsta, DataManut, IDUsuario, Nome)
               VALUES ('$mac',$NBoias,$CapComp,'$MarcaComp','$DataInsta','$DataManut','$IDUser','$Nome')"; // Insere no DB
    mysqli_query($conn, $sql);

    // if(mysqli_query($conn, $sql)){
    //   echo "done";                                  // Responde sucesso
    // }
    // header( "Location: {$_SERVER['REQUEST_URI']}", true, 303 );  // Método PRG 303-> resposta padrão pra form
    // exit();

    $conn->close();
}
//     echo json_encode(array('successo' => $IDuser));
// } else {
//     echo json_encode(array('success' => 0));
// }
?>
