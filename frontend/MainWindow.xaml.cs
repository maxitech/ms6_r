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
using System.IO;


namespace frontend_v2;

/// <summary>
/// Interaction logic for MainWindow.xaml
/// </summary>
public partial class MainWindow : Window
{
    private ManagementEventWatcher? _watcher;
    private SerialPort? _serialPort;


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
        Dispatcher.InvokeAsync(() =>
        {
            string? selectedPort = comboBoxPorts.SelectedItem?.ToString();

            comboBoxPorts.Items.Clear();
            List<string> availablePorts = LoadAvailablePorts();
            RenderPorts(availablePorts);

            // Check if previously selected port is still available
            if (selectedPort != null && availablePorts.Contains(selectedPort))
            {
                comboBoxPorts.SelectedItem = selectedPort;
            }
            else
            {
                if (_serialPort != null && _serialPort.IsOpen) _serialPort.Close();

                // Update UI
                comboBoxPorts.SelectedItem = null;
                ResetUIOnConnectionFailure();
            }
        });
    }


    protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
    {
        // Stop and dispose of the watcher if it exists
        if (_watcher != null)
        {
            try
            {
                _watcher.Stop();
                _watcher.Dispose();

            }
            catch { /* Ignore */  }
        }

        // Close the serial port if it is open
        if (_serialPort != null && _serialPort.IsOpen)
        {
            try
            {
                _serialPort.Close();
            }
            catch { /* Ignore */ }
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
        if (comboBoxPorts.SelectedItem != null) ConnectBtn.IsEnabled = true;
    }


    private async Task InitSerialPort(string portName)
    {
        if (_serialPort != null && _serialPort.IsOpen) return;


        // Create new serial port instance if not already created
        _serialPort = new SerialPort(portName, 9600, Parity.None, 8, StopBits.One)
        {
            Handshake = Handshake.None,
            ReadTimeout = 500,
            WriteTimeout = 500
        };

        _serialPort.DataReceived += SerialPortDataReceived;

        try
        {
            _serialPort.Open();

            if (_serialPort.IsOpen)
            {
                // Update UI
                await Dispatcher.BeginInvoke(() =>
                 {
                     ConnectionStatus.Content = "Connected";
                     ConnectionStatus.Background = Brushes.Green;
                     ConnectBtn.IsEnabled = true;
                     ConnectBtn.Content = "Disconnect";
                 });
            }

        }
        catch (UnauthorizedAccessException)
        {
            ResetUIOnConnectionFailure();
            MessageBox.Show("The selected COM port is already in use or requires elevated permissions. Please check your settings.",
                   "Access Denied", MessageBoxButton.OK, MessageBoxImage.Error);
        }
        catch (IOException)
        {
            ResetUIOnConnectionFailure();
            MessageBox.Show("An unexpected error occurred while accessing the COM port. Ensure the device is connected properly.",
                    "I/O Error", MessageBoxButton.OK, MessageBoxImage.Error);
        }
        catch (ArgumentException)
        {
            ResetUIOnConnectionFailure();
            MessageBox.Show("Invalid COM port name. Please select a valid port.",
                            "Invalid Port", MessageBoxButton.OK, MessageBoxImage.Error);
        }
        catch (InvalidOperationException)
        {
            ResetUIOnConnectionFailure();
            MessageBox.Show("The serial port is already open. Please close the existing connection before opening a new one.",
                            "Operation Error", MessageBoxButton.OK, MessageBoxImage.Error);
        }
        catch (SystemException)
        {
            ResetUIOnConnectionFailure();
            MessageBox.Show("A system error occurred while opening the port. Restarting your computer may help.",
                            "System Error", MessageBoxButton.OK, MessageBoxImage.Error);
        }
        catch (Exception ex)
        {
            ResetUIOnConnectionFailure();
            MessageBox.Show($"Error while opening serial port:\n {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
        }
    }


    private async Task SendDataAsync(string data)
    {
        try
        {
            if (_serialPort != null && !_serialPort.IsOpen)
            {
                await Task.Run(() => _serialPort.WriteLine(data));
            }
            else
            {
                MessageBox.Show("Serial port is not open", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
        catch (Exception ex)
        {
            MessageBox.Show($"Error while sending data:\n {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
        }
    }


    private async void SerialPortDataReceived(object sender, SerialDataReceivedEventArgs e)
    {
        if (_serialPort == null || !_serialPort.IsOpen) return;

        string data = await Task.Run(() => _serialPort.ReadExisting());
        Dispatcher.Invoke(() =>
        {
            // Update UI here -> show received data
        });
    }


    private void ConnectBtn_Click(object sender, RoutedEventArgs e)
    {
        if (_serialPort != null && _serialPort.IsOpen)
        {
            // Disconnect
            _serialPort.Close();

            // Update UI
            ConnectionStatus.Content = "Disconnected";
            ConnectionStatus.Background = Brushes.Red;
            ConnectBtn.Content = "Connect";
            comboBoxPorts.IsEnabled = true;
        }
        else
        {
            // Connect
            string? selectedPort = comboBoxPorts.SelectedItem?.ToString()?.Split(' ')[0];
            if (selectedPort == null) return;

            // Update UI
            ConnectionStatus.Content = "Connecting";
            ConnectionStatus.Background = Brushes.Yellow;
            ConnectBtn.IsEnabled = false;
            ConnectBtn.Content = "Disconnect";
            comboBoxPorts.IsEnabled = false;

            InitSerialPort(selectedPort).ContinueWith(t =>
            {
                if (t.IsFaulted)
                {
                    ResetUIOnConnectionFailure();
                }
            });
        }
    }


    private void ResetUIOnConnectionFailure()
    {
        Dispatcher.BeginInvoke(() =>
        {
            ConnectionStatus.Content = "Disconnected";
            ConnectionStatus.Background = Brushes.Red;
            ConnectBtn.IsEnabled = false;
            ConnectBtn.Content = "Connect";
            comboBoxPorts.IsEnabled = true;
        });
    }
}