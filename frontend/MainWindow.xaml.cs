using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Management;
using System.Threading.Tasks;


namespace frontend_v2;

/// <summary>
/// Interaction logic for MainWindow.xaml
/// </summary>
public partial class MainWindow : Window
{
    private ManagementEventWatcher _watcher;


    public MainWindow()
    {
        InitializeComponent();

        // Load and render available ports
        RenderPorts(LoadAvailablePorts());

        // Start listening for device changes
        StartWmiListener();

    }


    private void StartWmiListener()
    {
        try
        {

            string query = "SELECT * FROM Win32_DeviceChangeEvent";

            _watcher = new ManagementEventWatcher(new WqlEventQuery(query));
            _watcher.EventArrived += OnDeviceChanged;
            _watcher.Start();
        }
        catch (Exception ex)
        {
            MessageBox.Show($"Error while starting WMI listener:\n {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
        }
    }


    private void OnDeviceChanged(object sender, EventArrivedEventArgs e)
    {
        Dispatcher.InvokeAsync(new Action(() =>
        {
            string selectedPort = comboBoxPorts.SelectedItem?.ToString();

            comboBoxPorts.Items.Clear();
            List<string> availablePorts = LoadAvailablePorts();
            RenderPorts(availablePorts);

            if (!availablePorts.Contains(selectedPort))
            {
                comboBoxPorts.SelectedItem = null;
                ConnectBtn.IsEnabled = false;
            }
        }));
    }


    protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
    {
        if (_watcher != null)
        {
            try
            {
                _watcher.Stop();
                _watcher.Dispose();

            }
            catch { /* Ignore */  }
        }
        base.OnClosing(e);
    }


    private static List<string> LoadAvailablePorts()
    {
        try
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
                List<string> portList = portnames.Select(n => n + " - " + ports.FirstOrDefault(s => s.Contains(n)) ?? "Unknown").ToList();

                return portList;
            }
        }
        catch (Exception ex)
        {
            MessageBox.Show($"Error while loading COM-ports:\n {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            return new List<string>();
        }
    }


    private void RenderPorts(List<string> availablePorts)
    {
        foreach (string port in availablePorts)
        {
            comboBoxPorts.Items.Add(port);
        }
    }

    private void ComboBoxPorts_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
        if (comboBoxPorts.SelectedItem != null)
        {
            ConnectBtn.IsEnabled = true;
        }
    }
}