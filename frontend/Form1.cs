using System.IO.Ports;
using System.Management;

namespace frontend
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            // Load and render available ports
            RenderPorts(LoadAvailablePorts());
        }
        private static List<string> LoadAvailablePorts()
        {
            // If interested in how this works, check out the following link: https://youtu.be/3SQayMiapKQ?si=ZZsX7aso8TKUQ_hf
            using (var searcher = new ManagementObjectSearcher("SELECT * FROM " +
                "Win32_PnPEntity WHERE Caption like '%(COM%'"))
            {
                // Get port names
                string[] portnames = SerialPort.GetPortNames();

                // Get caption of each port
                var ports = searcher.Get().Cast<ManagementBaseObject>().ToList().Select(p => p["Caption"].ToString());

                // Combine port names with captions
                List<string> portList = portnames.Select(n => n + " - " + ports.FirstOrDefault(s => s.Contains(n))).ToList();

                return portList;
            }
        }

        private void RenderPorts(List<string> availablePorts)
        {
            foreach (string port in availablePorts)
            {
                comboBox1.Items.Add(port);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
