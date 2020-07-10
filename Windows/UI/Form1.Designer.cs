namespace UI
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.notifyIcon = new System.Windows.Forms.NotifyIcon(this.components);
            this.contextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.甘ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.文ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.崔ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // notifyIcon
            // 
            this.notifyIcon.BalloonTipIcon = System.Windows.Forms.ToolTipIcon.Info;
            this.notifyIcon.ContextMenuStrip = this.contextMenuStrip;
            this.notifyIcon.Icon = ((System.Drawing.Icon)(resources.GetObject("notifyIcon.Icon")));
            this.notifyIcon.Text = "notifyIcon";
            this.notifyIcon.Visible = true;
            // 
            // contextMenuStrip
            // 
            this.contextMenuStrip.ImageScalingSize = new System.Drawing.Size(40, 40);
            this.contextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.甘ToolStripMenuItem,
            this.文ToolStripMenuItem,
            this.崔ToolStripMenuItem});
            this.contextMenuStrip.Name = "contextMenuStrip";
            this.contextMenuStrip.Size = new System.Drawing.Size(361, 197);
            // 
            // 甘ToolStripMenuItem
            // 
            this.甘ToolStripMenuItem.Name = "甘ToolStripMenuItem";
            this.甘ToolStripMenuItem.Size = new System.Drawing.Size(360, 46);
            this.甘ToolStripMenuItem.Text = "甘";
            this.甘ToolStripMenuItem.Click += new System.EventHandler(this.甘ToolStripMenuItem_Click);
            // 
            // 文ToolStripMenuItem
            // 
            this.文ToolStripMenuItem.Name = "文ToolStripMenuItem";
            this.文ToolStripMenuItem.Size = new System.Drawing.Size(360, 46);
            this.文ToolStripMenuItem.Text = "文";
            this.文ToolStripMenuItem.Click += new System.EventHandler(this.文ToolStripMenuItem_Click);
            // 
            // 崔ToolStripMenuItem
            // 
            this.崔ToolStripMenuItem.Name = "崔ToolStripMenuItem";
            this.崔ToolStripMenuItem.Size = new System.Drawing.Size(360, 46);
            this.崔ToolStripMenuItem.Text = "崔";
            this.崔ToolStripMenuItem.Click += new System.EventHandler(this.崔ToolStripMenuItem_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(15F, 30F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Opacity = 0D;
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "Form1";
            this.WindowState = System.Windows.Forms.FormWindowState.Minimized;
            this.Load += new System.EventHandler(this.Form1_Load);
            this.contextMenuStrip.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.NotifyIcon notifyIcon;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem 甘ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 文ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 崔ToolStripMenuItem;
    }
}

