<?php
$on =$_POST['on'];
$id =$_POST['id'];
include 'acess.php';
$conn = con();
$sql = "UPDATE aerador SET ligado= $on WHERE IDAerador=$id" ; // Insere no DB
mysqli_query($conn, $sql);
// echo $user;
$conn->close();
?>
