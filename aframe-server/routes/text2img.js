import express from "express";
import { text2imgHandler } from "../controllers/postImg.js";

const router = express.Router();
//text2img/postImg
router.post("/postImg", text2imgHandler);
export default router;
