namespace lw4
{
    partial class CalculateCurrencyRates
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.inputFilePath = new System.Windows.Forms.TextBox();
            this.isAsyncCheckbox = new System.Windows.Forms.CheckBox();
            this.textarea = new System.Windows.Forms.RichTextBox();
            this.requestButton = new System.Windows.Forms.Button();
            this.SuspendLayout();

            this.inputFilePath.Location = new System.Drawing.Point(55, 42);
            this.inputFilePath.Name = "inputFilePath";
            this.inputFilePath.Size = new System.Drawing.Size(431, 22);
            this.inputFilePath.TabIndex = 0;

            this.isAsyncCheckbox.AutoSize = true;
            this.isAsyncCheckbox.Location = new System.Drawing.Point(55, 99);
            this.isAsyncCheckbox.Name = "isAsyncCheckbox";
            this.isAsyncCheckbox.Size = new System.Drawing.Size(114, 21);
            this.isAsyncCheckbox.TabIndex = 3;
            this.isAsyncCheckbox.Text = "AsyncRequest";
            this.isAsyncCheckbox.UseVisualStyleBackColor = true;
            this.isAsyncCheckbox.CheckedChanged += new System.EventHandler(this.isAsyncCheckbox_CheckedChanged);

            this.textarea.Location = new System.Drawing.Point(55, 261);
            this.textarea.Name = "textarea";
            this.textarea.Size = new System.Drawing.Size(456, 158);
            this.textarea.TabIndex = 4;
            this.textarea.Text = "";

            this.requestButton.Location = new System.Drawing.Point(55, 150);
            this.requestButton.Name = "syncRequestButton";
            this.requestButton.Size = new System.Drawing.Size(150, 50);
            this.requestButton.TabIndex = 5;
            this.requestButton.Text = "Request";
            this.requestButton.UseVisualStyleBackColor = true;
            this.requestButton.Click += new System.EventHandler(this.requestButton_Click);

            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 600);
            this.Controls.Add(this.requestButton);
            this.Controls.Add(this.textarea);
            this.Controls.Add(this.isAsyncCheckbox);
            this.Controls.Add(this.inputFilePath);
            this.Name = "CalculateCurrencyRates";
            this.Text = "lw4";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox inputFilePath;
        private System.Windows.Forms.CheckBox isAsyncCheckbox;
        private System.Windows.Forms.RichTextBox textarea;
        private System.Windows.Forms.Button requestButton;
    }
}