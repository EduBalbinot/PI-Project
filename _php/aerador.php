<!doctype html>
<html>
<head>
  <meta name="google" content="notranslate" />
  <link rel="stylesheet" href="/_css/main.css">
  <link rel="stylesheet" href="/_css/switch.css">
  <link rel="stylesheet" href="/_css/grafico.css">
  <link rel="stylesheet" href="/_css/dcSlider.css">
  <script src="https://code.jquery.com/jquery-3.3.1.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/moment"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/3.9.1/chart.min.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/chartjs-adapter-moment"></script>
</head>
<body>

<div id="main">
  <?php include "menu.php"; ?>
  <?php
    $IDAerador=sessaoAerador();
    [$data,$press,$temp,$infoAerador,$leitura] = tabelaLeitura($IDAerador);
 ?>
 <div class="info">
  <?php
  $estado="Desligado";
  if($leitura["EstadoComp"]) $estado="Ligado";
    echo "<div class=\"informaçoes\">
    <h3>" .  $infoAerador["Nome"]   .                        "</h3>
    <p>ID:" .  $infoAerador["IDAerador"]   .                        "</p>
    <p>Número de bóias: " .  $infoAerador["NBoias"]   .       " bóias</p>
    <p>Capacidade: " .  $infoAerador["CapComp"]   .              " m³</p>
    <p>Marca: " .  $infoAerador["MarcaComp"]   .                    "</p>
    <p>Frequencia: " .  $infoAerador["freq"]   .                    "</p>
    <p>MAC: " .  $infoAerador["MacAddress"]   .                     "</p>
    <p>Data Instalação: " .  $infoAerador["DataInsta"]   .          "</p>
    <p>Data Última Manutenção: " .  $infoAerador["DataManut"]   .   "</p></div>
    <div class=\"leitura\">
    <h4>Última leitura</h4>
    <p>Data: " .  $leitura["Data"]   .   "</p>
    <p>Pressão: " .  $leitura["Vazao"]   .   " mPa</p>
    <p>Estado Compressor: " .  $estado   .   "</p>
    <p>Duty Cycle: " .  $leitura["DutyCycle"]   .   "</p>
    <p>rssI: " .  $leitura["rssI"]   .   "</p></div>"
    ?>
    <div class="boxSwitch">
      <label class="switch">
        <input id="checkbox" type="checkbox" onclick=alteraEstado(this.id)>
        <span class="slider round"></span>
        </label>
      <h2 id="EstadoAerador" style="color:red"> Aerador Desligado </h2>
    </div>
 <form id="dcCustom" method="post">
   <?php
      $DC= json_decode($infoAerador["DutyCycle"]);
     for($i=0; $i<=12; $i++){
       $a=$i*2;
       echo "<div class=\"dcSlider\">";
       echo "<p>".$a."h</p>";
       echo "<input id=\"slider".$i."\" name=\"".$i."\"type=\"range\" min=\"0\" max=\"1\" value=".$DC[$i]." step=\"0.05\" oninput=\"this.nextElementSibling.value = this.value\">";
       echo "<output>".$DC[$i]."</output>";
       echo "</div>";
     }
   ?>
   <input type="submit" value="Alterar">
 </form>
 </div>
 <div class="boxGraficos">
   <div class="boxGrafico Temp">
     <canvas class="grafico" id="graficoTemp"></canvas>
   </div>
   <div class="boxGrafico Pressao">
     <canvas class="grafico" id="graficoPressao"></canvas>
  </div>
 </div>
 </div>
 <script type="text/javascript">
   $(document).ready(function() {
       $('#dcCustom').submit(function(e) {
           e.preventDefault();
           // alert($(this).serialize());
           $.ajax({
               type: "POST",
               url: 'DCAlter.php',
               data: $(this).serialize(),
               success: function(response)
               {
                    var jsonData = JSON.parse(response);
                    // alert(response);
              }
          });
        });
      });
</script>
<script>
window.onload = function() {
      if(<?php switchOn($IDAerador) ?>){
        document.getElementById("checkbox").checked = true;
        document.getElementById("EstadoAerador").textContent = "Aerador Ligado";
        document.getElementById("EstadoAerador").style.color="green";
      };
      document.getElementById("<?php echo $IDAerador ?>").style.backgroundColor="rgb(0,0,200,0.15)";
      document.getElementById("<?php echo $IDAerador ?>").style.border="1px solid black";
      var ctx1 = document.getElementById('graficoTemp').getContext('2d');
      var ctx2 = document.getElementById('graficoPressao').getContext('2d');
      var config1={
        type: 'line',
        data: {
          datasets: [{
            label: 'Temperatura',
            backgroundColor: 'red',
            borderColor: 'red',
            data: [
              <?php if(empty($data)>0){
              echo "{ x: '0', y: 0 },";
            } else {
              for ($i = 0; $i < count($data); $i++) {
                echo "{ x: '".$data[$i]."', y: ".$temp[$i]." },";
              }
            }
            ?>
            ]
          }]
        },
        options: {
          scales: {
            y: {
              round: false,
              title: {
                      display: true,
                      text: 'Temperatura [°C]'
                    }
                },
            x: {
              title: {
                      display: true,
                      text: 'Hora'
                    },
              type: 'time',
              time: {
                    unit: 'hour',
                    displayFormats: { 'hour': 'H:mm' },
                    round: false,
                },
              min: '<?php echo date('Y-m-j');?> 00:00:00',
              max: '<?php echo date('Y-m-j');?> 24:00:00',
            }
          },
          interaction: {
            intersect: false,
            mode: 'nearest',
            axis: 'x'
                      },
          plugins: {
            tooltip: {
                callbacks: {
                    label: function(context) {
                      let label = context.dataset.label || '';
                      if (context.parsed.y !== null) {
                          label = context.parsed.y + ' °C';
                      }
                      return label;
                    }
                }
            }
        }
        }
      }

      var config2={
        type: 'line',
        data: {
          datasets: [{
            label: 'Pressão',
            backgroundColor: 'green',
            borderColor: 'green',
            data: [
              <?php if(empty($data)>0){
              echo "{ x: '0', y: 0 },";
            } else {
              for ($i = 0; $i < count($data); $i++) {
                echo "{ x: '".$data[$i]."', y: ".$press[$i]." },";
              }
            }
            ?>
            ]
          }]
        },
        options: {
          scales: {
            y: {
              round: false,
              title: {
                      display: true,
                      text: 'Pressão [mPa]'
                    }
            },
            x: {
              title: {
                      display: true,
                      text: 'Hora'
                    },
              type: 'time',
              time: {
                    unit: 'hour',
                    displayFormats: { 'hour': 'H:mm' },
                    round: false,
                },
              min: '<?php echo date('Y-m-j');?> 00:00:00',
              max: '<?php echo date('Y-m-j');?> 24:00:00',
            }
          },
          interaction: {
            intersect: false,
            mode: 'nearest',
            axis: 'x'
                      },
          plugins: {
            tooltip: {
                callbacks: {
                    label: function(context) {
                      let label = context.dataset.label || '';
                      if (context.parsed.y !== null) {
                          label = context.parsed.y + ' mPA';
                      }
                      return label;
                    }
                }
            }
        }
        }
      }
      var myChart = new Chart(ctx1, config1);
      var myChart = new Chart(ctx2, config2);
    };

    function alteraEstado(a){
      if(document.getElementById("checkbox").checked == true){
        document.getElementById("EstadoAerador").textContent = "Aerador Ligado";
        document.getElementById("EstadoAerador").style.color="green";
      } else {
        document.getElementById("EstadoAerador").textContent = "Aerador Desligado";
        document.getElementById("EstadoAerador").style.color="red";
      }
      $.ajax({
          type: "POST",
          url: 'delete.php',
          data: {on: $("#"+ a).is(':checked'), id: <?php echo $IDAerador ?>},
          success: function(response)
          {
         }
     });
   };
</script>
</body>
</html>
