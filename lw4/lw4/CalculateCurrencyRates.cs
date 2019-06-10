using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lw4
{
    public partial class CalculateCurrencyRates : Form
    {
        private bool isAsync = false;
        private Stopwatch timeCounter;

        public CalculateCurrencyRates()
        {
            InitializeComponent();
        }

        public void requestButton_Click(object sender, EventArgs e)
        {
            try
            {
                sendRequest();
            }
            catch (Exception ex)
            {
                textarea.Text = ex.Message;
            }
        }

        public void isAsyncCheckbox_CheckedChanged(object sender, EventArgs e)
        {
            isAsync = !isAsync;
        }

        private void sendRequest()
        {
            if (isAsync)
            {
                runAsync();
            }
            else
            {
                runSync();
            }
        }

        private void runAsync()
        {
            timeCounter = new Stopwatch();
            timeCounter.Start();

            string filePath = this.inputFilePath.Text;
            FileCurrencyReader reader = new FileCurrencyReader(filePath);
            var expectedCurrency = reader.ReadSet();

            var executer = new RequestExecuter();
            string response = executer.RequestAsync("https://www.cbr-xml-daily.ru/daily_json.js").Result;

            JObject jsonReponse = JObject.Parse(response);

            string result = "";
            foreach (var valute in jsonReponse["Valute"])
            {
                var currency = valute.First();
                if (expectedCurrency.Contains(currency["CharCode"].ToString()))
                {
                    var nominal = currency["Nominal"].ToString();
                    var name = currency["Name"].ToString();
                    var value = currency["Value"].ToString();
                    result += $"{nominal} {name} по курсу {value}\n";
                }
            }

            result += "Time: " + timeCounter.ElapsedMilliseconds + "ms";
            textarea.Text = result;
            timeCounter.Stop();
        }

        private void runSync()
        {
            timeCounter = new Stopwatch();
            timeCounter.Start();

            string filePath = this.inputFilePath.Text;
            FileCurrencyReader reader = new FileCurrencyReader(filePath);
            var expectedCurrency = reader.ReadSet();

            var executer = new RequestExecuter();
            string response = executer.Request("https://www.cbr-xml-daily.ru/daily_json.js");

            JObject jsonReponse = JObject.Parse(response);

            string result = "";
            foreach (var currency in jsonReponse["Valute"])
            {
                var val = currency.First();
                if (expectedCurrency.Contains(val["CharCode"].ToString()))
                {
                    var nominal = val["Nominal"].ToString();
                    var name = val["Name"].ToString();
                    var value = val["Value"].ToString();
                    result += $"{nominal} {name} по курсу {value}\n";
                }
            }
            result += "Time: " + timeCounter.ElapsedMilliseconds + "ms";
            textarea.Text = result;
            timeCounter.Stop();
        }
    }
}
