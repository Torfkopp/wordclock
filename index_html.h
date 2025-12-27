#ifndef INDEX_HTML_H
#define INDEX_HTML_H

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Wortuhr Einstellungen</title>
    <style>
        :root { --primary: #00ffaa; --bg: #111; --card: #222; --text: #eee; }
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; background: var(--bg); color: var(--text); margin: 0; padding: 20px; }
        .container { max-width: 600px; margin: 0 auto; }
        h1 { text-align: center; color: var(--primary); }
        .card { background: var(--card); padding: 20px; border-radius: 12px; margin-bottom: 20px; box-shadow: 0 4px 6px rgba(0,0,0,0.3); }
        .form-group { margin-bottom: 15px; }
        label { display: block; margin-bottom: 5px; font-weight: bold; }
        input[type="text"], input[type="password"], input[type="number"], input[type="time"], select { width: 100%; padding: 10px; border-radius: 6px; border: 1px solid #444; background: #333; color: white; box-sizing: border-box; }
        input[type="color"] { width: 100%; height: 40px; border: none; padding: 0; background: none; }
        button { width: 100%; padding: 12px; background: var(--primary); border: none; border-radius: 6px; color: #000; font-weight: bold; cursor: pointer; font-size: 16px; transition: opacity 0.2s; }
        button:hover { opacity: 0.9; }
        .row { display: flex; gap: 10px; }
        .col { flex: 1; }
        .checkbox-group { display: flex; flex-wrap: wrap; gap: 10px; }
        .checkbox-item { background: #333; padding: 5px 10px; border-radius: 4px; display: flex; align-items: center; gap: 5px; cursor: pointer; user-select: none; }
        .checkbox-item input { margin: 0; }
        
        /* Toggle Switch */
        .switch { position: relative; display: inline-block; width: 50px; height: 24px; }
        .switch input { opacity: 0; width: 0; height: 0; }
        .slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #444; transition: .4s; border-radius: 24px; }
        .slider:before { position: absolute; content: ""; height: 16px; width: 16px; left: 4px; bottom: 4px; background-color: white; transition: .4s; border-radius: 50%; }
        input:checked + .slider { background-color: var(--primary); }
        input:checked + .slider:before { transform: translateX(26px); }
    </style>
</head>
<body>
    <div class="container">
        <h1>Wortuhr</h1>
        
        <div class="card">
            <h2>Aussehen</h2>
            <div class="form-group">
                <label>Farbe</label>
                <input type="color" id="color" value="#00ffaa">
            </div>
            <div class="row">
                <div class="col form-group">
                    <label>Tag Helligkeit</label>
                    <input type="number" id="br_day" min="0" max="255">
                </div>
                <div class="col form-group">
                    <label>Nacht Helligkeit</label>
                    <input type="number" id="br_night" min="0" max="255">
                </div>
            </div>
        </div>

        <div class="card">
            <h2>Nachtmodus (Dimmen)</h2>
            <div class="row">
                <div class="col form-group">
                    <label>Startstunde</label>
                    <input type="number" id="n_start" min="0" max="23">
                </div>
                <div class="col form-group">
                    <label>Endestunde</label>
                    <input type="number" id="n_end" min="0" max="23">
                </div>
            </div>
        </div>

        <div class="card">
            <h2>Zeitplan fürs Ausschalten</h2>
            <p style="font-size: 0.9em; color: #aaa;">Wähle die Tage, an denen die Lichter ausgehen sollen</p>
            <div class="form-group checkbox-group" id="days-container">
                <!-- JS will populate -->
            </div>
            <div class="row">
                <div class="col form-group">
                    <label>Startstunde</label>
                    <input type="number" id="o_start" min="0" max="23">
                </div>
                <div class="col form-group">
                    <label>Endstunde</label>
                    <input type="number" id="o_end" min="0" max="23">
                </div>
            </div>
        </div>
        
        <div class="card">
             <h2>Options</h2>
             <div class="form-group" style="display: flex; justify-content: space-between; align-items: center;">
                <label style="margin:0">Zeig Datum alle 30 min</label>
                <label class="switch">
                  <input type="checkbox" id="show_date">
                  <span class="slider"></span>
                </label>
            </div>
        </div>

        <button onclick="saveSettings()">Einstellungen speichern</button>
    </div>

    <script>
        const days = ['Mo', 'Di', 'Mi', 'Do', 'Fr', 'Sa', 'So'];
        
        function initDays() {
            const container = document.getElementById('days-container');
            days.forEach((day, idx) => {
                const div = document.createElement('div');
                div.className = 'checkbox-item';
                div.innerHTML = `<input type="checkbox" id="day_${idx}" value="${idx}"> ${day}`;
                container.appendChild(div);
            });
        }

        async function loadSettings() {
            try {
                const res = await fetch('/api/settings');
                const data = await res.json();
                
                // Color Int to Hex
                const hex = '#' + (data.color & 0x00FFFFFF).toString(16).padStart(6, '0');
                document.getElementById('color').value = hex;
                
                document.getElementById('br_day').value = data.br_day;
                document.getElementById('br_night').value = data.br_night;
                document.getElementById('n_start').value = data.n_start;
                document.getElementById('n_end').value = data.n_end;
                document.getElementById('o_start').value = data.o_start;
                document.getElementById('o_end').value = data.o_end;
                document.getElementById('show_date').checked = data.show_date;

                // Bitmask
                for(let i=0; i<7; i++) {
                    document.getElementById('day_' + i).checked = (data.off_days >> i) & 1;
                }
            } catch(e) { console.error(e); }
        }

        async function saveSettings() {
            const data = new FormData();
            
            // Hex to Int
            const hex = document.getElementById('color').value;
            const colInt = parseInt(hex.substring(1), 16);
            data.append('color', colInt);
            
            data.append('br_day', document.getElementById('br_day').value);
            data.append('br_night', document.getElementById('br_night').value);
            data.append('n_start', document.getElementById('n_start').value);
            data.append('n_end', document.getElementById('n_end').value);
            data.append('o_start', document.getElementById('o_start').value);
            data.append('o_end', document.getElementById('o_end').value);
            data.append('show_date', document.getElementById('show_date').checked ? '1' : '0');

            let mask = 0;
            for(let i=0; i<7; i++) { // 0=Mo
                if(document.getElementById('day_' + i).checked) mask |= (1 << i);
            }
            data.append('off_days', mask);

            await fetch('/api/settings', { method: 'POST', body: data });
            alert('Saved!');
        }

        initDays();
        loadSettings();
    </script>
</body>
</html>
)rawliteral";

#endif
