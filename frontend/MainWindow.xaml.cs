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


    /// <summary>
    /// Starts the WMI listener to detect device change events.
    /// </summary>
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


    /// <summary>
    /// Handles device change events by reloading the list of available COM ports.
    /// If the previously selected port is not available, it disconnects the serial port.
    /// </summary>
    /// <param name="sender">The source of the event.</param>
    /// <param name="e">The event data.</param>
    private void OnDeviceChanged(object sender, EventArrivedEventArgs e)
    {
        Dispatcher.InvokeAsync(() =>
        {
            string? selectedPort = comboBoxPorts.SelectedItem?.ToString();

            // Clear the existing list of ports.
            comboBoxPorts.Items.Clear();

            List<string> availablePorts = LoadAvailablePorts();
            availablePorts = availablePorts.Distinct().ToList();
            RenderPorts(availablePorts);

            // If the previously selected port is still available, re-select it.
            if (selectedPort != null && availablePorts.Contains(selectedPort))
            {
                comboBoxPorts.SelectedItem = selectedPort;
            }
            else
            {
                // Close the serial port if the selected port is no longer available.
                if (_serialPort != null && _serialPort.IsOpen) _serialPort.Close();

                // Update UI
                comboBoxPorts.SelectedItem = null;
                ResetUIOnConnectionFailure();
            }
        });
    }


    /// <summary>
    /// Overrides the OnClosing method to clean up resources.
    /// Stops the WMI watcher and closes the serial port if it is open.
    /// </summary>
    /// <param name="e">Provides data for canceling the closing event.</param>
    protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
    {
        // Stop and dispose of the WMI watcher if it exists.
        if (_watcher != null)
        {
            try
            {
                _watcher.Stop();
                _watcher.Dispose();

            }
            catch { /* Ignore exceptions on cleanup. */ }
        }

        // Close the serial port if it is open
        if (_serialPort != null && _serialPort.IsOpen)
        {
            try
            {
                _serialPort.Close();
            }
            catch { /* Ignore exceptions on cleanup. */ }
        }

        base.OnClosing(e);
    }


    /// <summary>
    /// Loads the available COM ports from the system.
    /// Combines the port names with their descriptive captions using WMI.
    /// </summary>
    /// <returns>A list of strings, each containing the port name and its caption.</returns>
    private static List<string> LoadAvailablePorts()
    {
        try
        {

            // If interested in how this works, check out the following link: https://youtu.be/3SQayMiapKQ?si=ZZsX7aso8TKUQ_hf
            using (var searcher = new ManagementObjectSearcher("SELECT * FROM " +
                "Win32_PnPEntity WHERE Caption like '%(COM%'"))
            {
                // Retrieve port names.
                string[] portNames = SerialPort.GetPortNames();

                // Retrieve captions for the ports.
                var portCaptions = searcher.Get()
                                    .Cast<ManagementBaseObject>()
                                    .ToList()
                                    .Select(p => p["Caption"]
                                    .ToString());

                // Filter and combine port names with their captions.
                List<string> filteredPorts = portNames
                    .Where(name => portCaptions.Any(caption => caption != null && caption.Contains(name)))
                    .Select(name => $"{name} - {portCaptions.First(caption => caption != null && caption.Contains(name))}")
                    .ToList();

                return filteredPorts;
            }
        }
        catch (Exception ex)
        {
            MessageBox.Show($"Error while loading COM-ports:\n {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            return new List<string>();
        }
    }


    /// <summary>
    /// Renders the available COM ports in the ComboBox.
    /// </summary>
    /// <param name="availablePorts">The list of available COM ports.</param>
    private void RenderPorts(List<string> availablePorts)
    {
        foreach (string port in availablePorts)
        {
            comboBoxPorts.Items.Add(port);
        }

    }


    /// <summary>
    /// Event handler for the ComboBox selection change.
    /// Enables the Connect button when a port is selected.
    /// </summary>
    /// <param name = "sender" > The source of the event.</param>
    /// <param name="e">The selection changed event data.</param>
    private void ComboBoxPorts_SelectionChanged(object sender, SelectionChangedEventArgs e)
    {
        if (comboBoxPorts.SelectedItem != null) ConnectBtn.IsEnabled = true;
    }


    /// <summary>
    /// Initializes and opens the serial port asynchronously.
    /// Configures the port settings and updates the UI on a successful connection.
    /// </summary>
    /// <param name="portName">The name of the COM port to open.</param>
    /// <returns>A Task representing the asynchronous operation.</returns>
    private async Task InitSerialPort(string portName)
    {
        if (_serialPort != null && _serialPort.IsOpen) return;


        // Create a new serial port instance with the specified settings.
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
                // Update the UI to reflect the successful connection.
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


    /// <summary>
    /// Sends data asynchronously to the connected serial port.
    /// Calls the AppendText method for updating the UI.
    /// </summary>
    /// <param name="data">The string data to send.</param>
    /// <param name="messagePrefix">The prefix to add to the message before sending.</param>
    /// <returns>A Task representing the asynchronous send operation.</returns>
    private async Task SendDataAsync(string data, string messagePrefix)
    {
        try
        {
            if (_serialPort != null && _serialPort.IsOpen)
            {
                await Task.Run(() => _serialPort.WriteLine(data));
                await Dispatcher.BeginInvoke(() =>
                {
                    if (messagePrefix == ">>") AppendText($"{messagePrefix} {data}", Brushes.DodgerBlue);
                    else
                        AppendText($"{messagePrefix} Load Program - {data}", Brushes.Blue);
                });
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


    /// <summary>
    /// Event handler for receiving data from the serial port.
    /// Reads available data asynchronously and updates the UI.
    /// </summary>
    /// <param name="sender">The source of the event.</param>
    /// <param name="e">The serial data received event data.</param>
    private async void SerialPortDataReceived(object sender, SerialDataReceivedEventArgs e)
    {
        if (_serialPort == null || !_serialPort.IsOpen) return;

        string data = await Task.Run(() => _serialPort.ReadExisting());
        await Dispatcher.BeginInvoke(() =>
        {
            // Update the UI to display the received data.
            AppendText($">> {data}", Brushes.DarkGreen);
        });
    }


    /// <summary>
    /// Handles the Connect button click event.
    /// Initiates a connection to the selected COM port or disconnects if already connected.
    /// </summary>
    /// <param name="sender">The source of the event.</param>
    /// <param name="e">The routed event data.</param>
    private void ConnectBtn_Click(object sender, RoutedEventArgs e)
    {
        if (_serialPort != null && _serialPort.IsOpen)
        {
            // Disconnect the serial port.
            _serialPort.Close();

            // Update the UI to reflect disconnection.
            ConnectionStatus.Content = "Disconnected";
            ConnectionStatus.Background = Brushes.Red;
            ConnectBtn.Content = "Connect";
            comboBoxPorts.IsEnabled = true;
        }
        else
        {
            // Connect to the selected COM port.
            string? selectedPort = comboBoxPorts.SelectedItem?.ToString()?.Split(' ')[0];
            if (selectedPort == null) return;

            // Update the UI to indicate connection in progress.
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


    /// <summary>
    /// Resets the UI elements to their default state after a connection failure.
    /// </summary>
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


    /// <summary>
    /// Handles the button click event for loading a program.
    /// Validates the user input and sends the program command if valid.
    /// </summary>
    /// <param name="sender">The source of the event.</param>
    /// <param name="e">Event data associated with the event.</param>
    private async void Button_Click_LoadProgram(object sender, RoutedEventArgs e)
    {
        var availablePrograms = new List<string> { "PING" }; // Add more programs here! Note:The program names should be in uppercase.
        string program = ProgramInput.Text.ToUpper();


        if (string.IsNullOrEmpty(program) || !availablePrograms.Contains(program))
        {
            MessageBox.Show("Please enter a valid program name. You can find the list of available Programs in the README.md file on GitHub. https://github.com/maxitech/ms6_r", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            return;
        }

        await SendDataAsync(program, "$");
    }


    /// <summary>
    /// Appends a message to the ComDisplay with the specified text color.
    /// </summary>
    /// <param name="message">The message to be displayed.</param>
    /// <param name="color">The color of the message text.</param>
    private void AppendText(string message, Brush color)
    {
        Run run = new Run(message + "\n")
        {
            Foreground = color
        };

        if (ComDisplay.Document.Blocks.FirstBlock is Paragraph paragraph)
        {
            paragraph.Inlines.Add(run);
        }

        ComDisplay.ScrollToEnd();
    }


    /// <summary>
    /// Handles the Window Loaded event.
    /// Initializes the UI by displaying a header message in red.
    /// </summary>
    /// <param name="sender">The source of the event.</param>
    /// <param name="e">Event data associated with the event.</param>
    private void Window_Loaded(object sender, RoutedEventArgs e)
    {
        AppendText("**SERIAL COMMUNICATION**", Brushes.Red);
    }
}