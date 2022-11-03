<!doctype html>
<html>
<head>
  <meta name="google" content="notranslate" />
  <!-- <link rel="stylesheet" href="/_css/main.css"> -->
  <link rel="stylesheet" href="/_css/colors.css">
  <link rel="stylesheet" href="/_css/overlay.css">
  <link rel="stylesheet" href="/_css/switch.css">
  <link rel="stylesheet" href="/_css/grid.css">
  <script src="https://code.jquery.com/jquery-3.3.1.js"></script>
</head>
<body>
<div id="main">
  <button id="botaoMostraCadastro" style="z-index: 2;position: fixed;"; onclick=mostraCadastro(this,1)></button>
  <?php include "menu.php"; ?>
  <div id="tabela">
    <?php
      $sessao = hiddenID();
      tabela($sessao);
      ?>
    </div>



<div id="overlayAerador" class="overlay">
  <div class="overlay-content">
    <button class="botaoFechaCadastro" ; onclick=fechaCadastro()></button>
    <h1>Novo Aerador</h1>
    <form id="loginform" method="post">
      <input type="hidden" name="idCadastro" id="idCadastro" value="100" />
      <p>Nome:              <input type="text" name="nomeCadastro"  id="nomeCadastro" />  </p>
      <p>Nº de boias:       <input type="text" name="nBoias"        id="nBoias" />        </p>
      <p>Capacidade         <input type="text" name="capacidade"    id="capacidade" />    </p>
      <p>Compressor:        <input type="text" name="marcaComp"     id="marcaComp" />     </p>
      <p>Frequência:        <input type="text" name="freq"          id="freq" />          </p>
      <input type="submit" name="inserir" id="inserir" value="Inserir" />
    </form>
  </div>
</div>
</div>

<script type="text/javascript">
$(document).ready(function() {
    $('#loginform').submit(function(e) {
        e.preventDefault();
        if(document.getElementById("inserir").value =="Inserir")
          document.getElementById("idCadastro").value ="-1";
        $.ajax({
            type: "POST",
            url: 'insert.php',
            data: $(this).serialize(),
           //  success: function(response)
           //  {
                // var jsonData = JSON.parse(response);
           // }
       });
       location.reload();
     });
});

function botaoDeleta(id) {
  $.ajax({
      type: "POST",
      url: 'delete.php',
      data: {data:id}
 });
       location.reload();
}

function fechaCadastro(){
  document.getElementById("overlayAerador").style.display = "none";
}

function mostraCadastro(botao, tipo){
  if(tipo==1) {
    document.getElementById("nomeCadastro").value   = "";
    document.getElementById("nBoias").value         = "";
    document.getElementById("capacidade").value     = "";
    document.getElementById("marcaComp").value      = "";
    document.getElementById("freq").value           = "";
    document.getElementById("inserir").value        = "Inserir";
}
  document.getElementById("overlayAerador").style.display  = "flex";
}

function botaoEdita(id) {
  document.getElementById("nomeCadastro").value   = document.getElementById("aerador"+id).cells[1].textContent;
  document.getElementById("nBoias").value         = document.getElementById("aerador"+id).cells[2].textContent;
  document.getElementById("capacidade").value     = document.getElementById("aerador"+id).cells[3].textContent;
  document.getElementById("marcaComp").value      = document.getElementById("aerador"+id).cells[4].textContent;
  document.getElementById("dataInsta").value      = document.getElementById("aerador"+id).cells[5].textContent;
  document.getElementById("inserir").value        = "Alterar";
  document.getElementById("idCadastro").value     = id;
  mostraCadastro(document.getElementById("botaoMostraCadastro"),0);
  }

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
</script>
</body>
</html>
