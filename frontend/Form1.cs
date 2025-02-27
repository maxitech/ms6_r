using System.IO.Ports;
using System.Management;

namespace frontend
{
    public partial class Form1 : Form
    {
        List<string> availablePorts = new List<string>();

        public Form1()
        {
            InitializeComponent();

            availablePorts = LoadAvailablePorts();
            foreach (string port in availablePorts)
            {
                comboBox1.Items.Add(port);
            }
        }
        private List<string> LoadAvailablePorts()
        {
            using (var searcher = new ManagementObjectSearcher("SELECT * FROM " +
                "Win32_PnPEntity WHERE Caption like '%(COM%'"))
            {
                string[] portnames = SerialPort.GetPortNames();

                var ports = searcher.Get().Cast<ManagementBaseObject>().ToList().Select(p => p["Caption"].ToString());

                List<string> portList = portnames.Select(n => n + " - " + ports.FirstOrDefault(s => s.Contains(n))).ToList();

                return portList;
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
