<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/_css/menu.css">
</head>
<body>

<div class="sidenav">
  <button class="menuMenuButton" id="voltarMenu" ; onclick=voltarMenu()>Menu</button>
    <?php
      include 'acess.php';
      $sessao = hiddenID();
      tabelaMenu($sessao);
    ?>
</div>

<script>
function botaoAerador(id) {
  $.ajax({
      type: "POST",
      url: 'sessionAerador.php',
      data: {id},
      success: function(response)
      {
          window.location='aerador.php';
     }
 });
}

function voltarMenu(){
  window.location='aeradores.php';
}

</script>
</body>
</html>
