<?php
$user = $_POST['data'];
include 'acess.php';
$conn = con();
$sql = "DELETE FROM aerador WHERE IDAerador= '$user' " ; // Insere no DB
mysqli_query($conn, $sql);
$conn->close();
?>
