
import express from "express";
import OpenAI from "openai";
import dotenv from "dotenv";

dotenv.config();

const app = express();

app.use(express.json({ limit: "20kb" }));
app.use(express.static("public"));

const client = new OpenAI({
  apiKey: process.env.OPENAI_API_KEY
});

app.post("/api/chat", async (req, res) => {
  try {
    const { message, history = [] } = req.body;

    if (
      typeof message !== "string" ||
      !message.trim()
    ) {
      return res.status(400).json({
        error: "پیام خالی است"
      });
    }

    const safeHistory = Array.isArray(history)
      ? history
          .filter(
            item =>
              item &&
              ["user", "assistant"].includes(item.role) &&
              typeof item.content === "string"
          )
          .slice(-20)
          .map(item => ({
            role: item.role,
            content: item.content.slice(0, 4000)
          }))
      : [];

    const response = await client.responses.create({
      model: "gpt-4.1-mini",

      instructions: `
تو یک دستیار هوش مصنوعی دوستانه و باهوش هستی.

قوانین:
1. پیام کاربر را بی‌دلیل تکرار نکن.
2. به سؤال کاربر مستقیماً پاسخ بده.
3. به گفتگوهای قبلی توجه کن.
4. اگر کاربر قبلاً نام یا علاقه‌ای گفته،
   در صورت مرتبط بودن از آن استفاده کن.
5. اگر چیزی را نمی‌دانی، صادقانه بگو.
6. به زبان کاربر پاسخ بده.
7. پاسخ‌ها طبیعی و واضح باشند.
`,

      input: [
        ...safeHistory,
        {
          role: "user",
          content: message
        }
      ]
    });

    res.json({
      answer: response.output_text
    });

  } catch (error) {
    console.error(error);

    res.status(500).json({
      error: "خطا در ارتباط با هوش مصنوعی"
    });
  }
});

const PORT = process.env.PORT || 3000;

app.listen(PORT, () => {
  console.log("AI server is running");
});
