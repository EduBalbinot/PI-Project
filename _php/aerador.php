<!doctype html>
<html>
<head>
  <meta name="google" content="notranslate" />
  <link rel="stylesheet" href="/_css/main.css">
  <script src="https://code.jquery.com/jquery-3.3.1.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>

  <?php include "menu.php"; ?>

<div id=main>

  <?php
    $IDAerador=sessaoAerador();
    [$hora,$vazao,$temp] = tabelaLeitura($IDAerador);
 ?>

 <div style="height: 40%; width: 50%;">
   <canvas id="graficoPressao"></canvas>
   <canvas id="graficoTemp"></canvas>
 </div>
</div>


<script>
  const labels = <?php echo json_encode($hora) ?>;

  const dataTemp = {
    labels: labels,
    datasets: [{
      label: 'Temperatura',
      backgroundColor: 'blue',
      borderColor: 'blue',
      data: <?php echo json_encode($temp) ?>,
    }]
  };

  const configTemp = {
    type: 'line',
    data: dataTemp,
    options: {}
  };

  const dataPressao = {
    labels: labels,
    datasets: [{
      label: 'Pressão',
      backgroundColor: 'red',
      borderColor: 'red',
      data: <?php echo json_encode($vazao) ?>,
    }]
  };

  const configPressao = {
    type: 'line',
    data: dataPressao,
    options: {
    }
  };
</script>
<script>
  const graficoPressao = new Chart(
    document.getElementById('graficoPressao'),
    configPressao
  );

  const graficoTemp = new Chart(
    document.getElementById('graficoTemp'),
    configTemp
  );
</script>
</body>
</html>
